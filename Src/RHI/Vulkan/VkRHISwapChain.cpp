//
// Created by arthur on 15/05/2024.
//

#include <array>
#include <Concerto/Core/Cast.hpp>

#include "Concerto/Graphics/RHI/Vulkan/VkRHISwapChain.hpp"

#include "Concerto/Graphics/RHI/Vulkan/VkRHIDevice.hpp"
#include "Concerto/Graphics/RHI/Vulkan/Utils.hpp"
#include "Concerto/Graphics/RHI/Vulkan/VKRHIRenderPass.hpp"

namespace Concerto::Graphics::RHI
{
	VkRHISwapChain::VkRHISwapChain(Vk::Device& device, Window& window, PixelFormat pixelFormat, PixelFormat depthPixelFormat) :
		RHI::SwapChain(pixelFormat, depthPixelFormat),
		Vk::SwapChain(device, window, Converters::ToVulkan(pixelFormat), Converters::ToVulkan(depthPixelFormat)),
		_pixelFormat(pixelFormat),
		_depthPixelFormat(depthPixelFormat)
	{
		CreateRenderPass();
		CreateFrameBuffers();
		_commandPool = std::make_unique<Vk::CommandPool>(device, device.GetQueueFamilyIndex(Vk::Queue::Type::Graphics));
		_presentQueue = std::make_unique<Vk::Queue>(device, device.GetQueueFamilyIndex(Vk::Queue::Type::Graphics));
		CreateFrames();
	}
	
	RHI::RenderPass& VkRHISwapChain::GetRenderPass()
	{
		CONCERTO_ASSERT(_renderPass, "ConcertoGraphics: Invalid renderpass");
		return *_renderPass;
	}

	Vector2u VkRHISwapChain::GetExtent()
	{
		return Vector2u{ Vk::SwapChain::GetExtent().width, Vk::SwapChain::GetExtent().height };
	}

	UInt32 VkRHISwapChain::GetImageCount()
	{
		return static_cast<UInt32>(Vk::SwapChain::GetImages().size());
	}

	Frame& VkRHISwapChain::AcquireFrame()
	{
		SwapChainFrame& frame = _frames[_currentFrameIndex];
		frame.Wait();
		if (_needResize)
		{
			Vk::SwapChain::ReCreate(Converters::ToVulkan(_pixelFormat), Converters::ToVulkan(_depthPixelFormat));
		}
		const UInt32 nextImageIndex = Vk::SwapChain::AcquireNextImage(frame.GetPresentSemaphore(), frame.GetRenderFence(), -1);
		frame.SetNextImageIndex(nextImageIndex);
		return frame;
	}

	Vk::CommandPool& VkRHISwapChain::GetCommandPool() const
	{
		CONCERTO_ASSERT(_commandPool, "ConcertoGraphics: Invalid command pool");
		return *_commandPool;
	}

	Vk::Device& VkRHISwapChain::GetDevice() const
	{
		CONCERTO_ASSERT(Vk::SwapChain::GetDevice(), "ConcertoGraphics: Invalid device");
		return *Vk::SwapChain::GetDevice();
	}

	Vk::Queue& VkRHISwapChain::GetPresentQueue() const
	{
		CONCERTO_ASSERT(_presentQueue, "ConcertoGraphics: Invalid present queue");
		return *_presentQueue;
	}

	void VkRHISwapChain::Present(UInt32 imageIndex)
	{
		_currentFrameIndex = (_currentFrameIndex + 1) % GetImageCount();
		SwapChainFrame& currentFrame = _frames[imageIndex];
		if (!_presentQueue->Present(currentFrame.GetRenderSemaphore(), *this, imageIndex))
		{
			switch (_presentQueue->GetLastResult())
			{
			case VK_ERROR_OUT_OF_DATE_KHR:
			case VK_SUBOPTIMAL_KHR:
			{
				_needResize = true;
				break;
			}
			default:
				{
					CONCERTO_ASSERT_FALSE("ConcertoGraphics: Present failed VKResult={}", static_cast<int>(_presentQueue->GetLastResult()));
				}
			}
		}
	}

	void VkRHISwapChain::CreateFrameBuffers()
	{
		const std::span<Vk::ImageView> imagesViews = Vk::SwapChain::GetImageViews();
		std::vector<VkImageView> vkImageViews;
		vkImageViews.reserve(imagesViews.size());
		for (auto& image: imagesViews)
			vkImageViews.push_back(*image.Get());

		_frameBuffers.reserve(vkImageViews.size());
		for (const VkImageView& imageView : vkImageViews)
		{
			std::vector attachments = {imageView, *GetDepthImageView().Get()};
			CONCERTO_ASSERT(attachments[0] != VK_NULL_HANDLE && attachments[1] != VK_NULL_HANDLE, "ConcertoGraphics: iInvalid attachment");

			Vk::FrameBuffer frameBuffer(*_device, static_cast<VkRHIRenderPass&>(*_renderPass), attachments, Vk::SwapChain::GetExtent());
			CONCERTO_ASSERT(frameBuffer.GetLastResult() == VK_SUCCESS, "ConcertoGraphics: Could not create framebuffer");

			_frameBuffers.emplace_back(std::move(frameBuffer));
		}
	}

	void VkRHISwapChain::CreateRenderPass()
	{
		std::vector<RHI::RenderPass::Attachment> attachment;
		auto& colorAttachment = attachment.emplace_back();
		colorAttachment.pixelFormat = GetPixelFormat();
		colorAttachment.loadOp = RHI::AttachmentLoadOp::Clear;
		colorAttachment.storeOp = RHI::AttachmentStoreOp::Store;
		colorAttachment.stencilLoadOp = RHI::AttachmentLoadOp::DontCare;
		colorAttachment.stencilStoreOp = RHI::AttachmentStoreOp::DontCare;
		colorAttachment.initialLayout = RHI::ImageLayout::Undefined;
		colorAttachment.finalLayout = RHI::ImageLayout::PresentSrcKhr;

		auto& depthAttachment = attachment.emplace_back();
		depthAttachment.pixelFormat = GetDepthPixelFormat();
		depthAttachment.loadOp = RHI::AttachmentLoadOp::Clear;
		depthAttachment.storeOp = RHI::AttachmentStoreOp::Store;
		depthAttachment.stencilLoadOp = RHI::AttachmentLoadOp::Clear;
		depthAttachment.stencilStoreOp = RHI::AttachmentStoreOp::DontCare;
		depthAttachment.initialLayout = RHI::ImageLayout::Undefined;
		depthAttachment.finalLayout = RHI::ImageLayout::DepthStencilAttachmentOptimal;

		std::vector<RHI::RenderPass::SubPassDescription> subPassDescriptions;

		auto& subPass = subPassDescriptions.emplace_back();
		subPass.colorAttachments.push_back({ 0, RHI::ImageLayout::ColorAttachmentOptimal });
		subPass.depthStencilAttachment = { 1, RHI::ImageLayout::DepthStencilAttachmentOptimal };


		std::vector<RHI::RenderPass::SubPassDependency> subPassDependencies;
		auto& colorDependency = subPassDependencies.emplace_back();
		colorDependency.srcSubPassIndex = RHI::RenderPass::SubPassDependency::ExternalSubPass;
		colorDependency.dstSubPassIndex = 0u;
		colorDependency.srcStageMask = static_cast<RHI::PipelineStageFlags>(RHI::PipelineStage::ColorAttachmentOutput);
		colorDependency.srcAccessFlags = 0u;
		colorDependency.dstStageMask = static_cast<RHI::PipelineStageFlags>(RHI::PipelineStage::ColorAttachmentOutput);
		colorDependency.dstAccessFlags = static_cast<RHI::MemoryAccessFlags>(RHI::MemoryAccess::ColorAttachmentWrite);

		auto& depthDependency = subPassDependencies.emplace_back();
		depthDependency.srcSubPassIndex = RHI::RenderPass::SubPassDependency::ExternalSubPass;
		depthDependency.dstSubPassIndex = 0u;
		depthDependency.srcStageMask = static_cast<RHI::PipelineStageFlags>(RHI::PipelineStage::EarlyFragmentTests) | static_cast<RHI::PipelineStageFlags>(RHI::PipelineStage::LateFragmentTests);
		depthDependency.srcAccessFlags = 0u;
		depthDependency.dstStageMask = static_cast<RHI::PipelineStageFlags>(RHI::PipelineStage::EarlyFragmentTests) | static_cast<RHI::PipelineStageFlags>(RHI::PipelineStage::LateFragmentTests);
		depthDependency.dstAccessFlags = static_cast<RHI::MemoryAccessFlags>(RHI::MemoryAccess::DepthStencilAttachmentWrite);

		_renderPass = Cast<VkRHIDevice&>(*_device).CreateRenderPass(attachment, subPassDescriptions, subPassDependencies);
		CONCERTO_ASSERT(_renderPass && Cast<VkRHIRenderPass&>(*_renderPass).GetLastResult() == VK_SUCCESS, "ConcertoGraphics: Could not create render pass");
	}

	void VkRHISwapChain::CreateFrames()
	{
		const UInt32 imageCount = GetImageCount();
		if (_frames.size() != imageCount)
		{
			_frames.clear();
			_frames.reserve(imageCount);
			for (Int32 i = 0; i < imageCount; ++i)
			{
				_frames.emplace_back(*this);
			}
		}
	}

	VkRHISwapChain::SwapChainFrame::SwapChainFrame(VkRHISwapChain& owner) :
		_commandBuffer(owner.GetCommandPool().AllocateCommandBuffer()),
		_renderFence(owner.GetDevice()),
		_presentSemaphore(owner.GetDevice()),
		_renderSemaphore(owner.GetDevice()),
		_owner(&owner),
		_imageIndex(0)
	{
	}

	void VkRHISwapChain::SwapChainFrame::Present()
	{
		const Vk::Queue& presentQueue = _owner->GetPresentQueue();

		presentQueue.Submit(_commandBuffer, _presentSemaphore, _renderSemaphore, _renderFence);
		_owner->Present(_imageIndex);
	}

	void VkRHISwapChain::SwapChainFrame::SetNextImageIndex(UInt32 imageIndex)
	{
		_imageIndex = imageIndex;
	}

	void VkRHISwapChain::SwapChainFrame::Wait()
	{
		_renderFence.Wait(-1);
		_renderFence.Reset();
	}

	Vk::Semaphore& VkRHISwapChain::SwapChainFrame::GetPresentSemaphore()
	{
		return _presentSemaphore;
	}

	Vk::Semaphore& VkRHISwapChain::SwapChainFrame::GetRenderSemaphore()
	{
		return _renderSemaphore;
	}

	Vk::Fence& VkRHISwapChain::SwapChainFrame::GetRenderFence()
	{
		return _renderFence;
	}
}
