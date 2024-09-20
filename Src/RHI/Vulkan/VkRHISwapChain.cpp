//
// Created by arthur on 15/05/2024.
//

#include <array>
#include <Concerto/Core/Cast.hpp>

#include "Concerto/Graphics/RHI/Vulkan/VkRHISwapChain.hpp"

#include "Concerto/Graphics/RHI/Vulkan/VkRHIDevice.hpp"
#include "Concerto/Graphics/RHI/Vulkan/Utils.hpp"
#include "Concerto/Graphics/RHI/Vulkan/VkRHIRenderPass.hpp"
#include "Concerto/Graphics/RHI/Vulkan/VkRHIDevice.hpp"
#include "Concerto/Graphics/RHI/Vulkan/VkRHICommandBuffer.hpp"
#include "Concerto/Graphics/RHI/Vulkan/VkRHICommandPool.hpp"
#include "Concerto/Graphics/RHI/Vulkan/VKRHITexture.hpp"

namespace Concerto::Graphics::RHI
{
	VkRHISwapChain::VkRHISwapChain(RHI::VkRHIDevice& device, Window& window, PixelFormat pixelFormat, PixelFormat depthPixelFormat) :
		RHI::SwapChain(pixelFormat, depthPixelFormat),
		Vk::SwapChain(device, window, Converters::ToVulkan(pixelFormat), Converters::ToVulkan(depthPixelFormat)),
		_pixelFormat(pixelFormat),
		_depthPixelFormat(depthPixelFormat)
	{
		CreateRenderPass();
		CreateFrameBuffers(device);
		_commandPool = device.CreateCommandPool(QueueFamily::Graphics);
		_presentQueue = std::make_unique<Vk::Queue>(device, device.GetQueueFamilyIndex(Vk::Queue::Type::Graphics));
		CreateFrames();
	}

	RHI::RenderPass& VkRHISwapChain::GetRenderPass()
	{
		CONCERTO_ASSERT(_renderPass, "ConcertoGraphics: Invalid renderpass");
		return *_renderPass;
	}

	Vector2u VkRHISwapChain::GetExtent() const
	{
		return Vector2u{ Vk::SwapChain::GetExtent().width, Vk::SwapChain::GetExtent().height };
	}

	UInt32 VkRHISwapChain::GetImageCount() const
	{
		return static_cast<UInt32>(Vk::SwapChain::GetImages().size());
	}

	Frame& VkRHISwapChain::AcquireFrame()
	{
		if (_needResize)
		{
			GetDevice()->WaitIdle();
			SwapChainFrame& frame = _frames[_currentFrameIndex];
			frame.Wait();
			Vk::SwapChain::ReCreate(Converters::ToVulkan(_pixelFormat), Converters::ToVulkan(_depthPixelFormat));
			_frameBuffers.clear();
			_frames.clear();
			CreateRenderPass();
			CreateFrames();
			CreateFrameBuffers(GetRHIDevice());
			_needResize = false;
		}
		SwapChainFrame& frame = _frames[_currentFrameIndex];
		frame.Wait();
		const UInt32 nextImageIndex = Vk::SwapChain::AcquireNextImage(frame.GetPresentSemaphore(), frame.GetRenderFence(), -1);
		frame.SetNextImageIndex(nextImageIndex);
		return frame;
	}

	VkRHICommandPool& VkRHISwapChain::GetCommandPool() const
	{
		CONCERTO_ASSERT(_commandPool, "ConcertoGraphics: Invalid command pool");
		return Cast<VkRHICommandPool&>(*_commandPool);
	}

	VkRHIDevice& VkRHISwapChain::GetRHIDevice() const
	{
		CONCERTO_ASSERT(Vk::SwapChain::GetDevice(), "ConcertoGraphics: Invalid device");
		return Cast<VkRHIDevice&>(*Vk::SwapChain::GetDevice());
	}

	Vk::Queue& VkRHISwapChain::GetPresentQueue() const
	{
		CONCERTO_ASSERT(_presentQueue, "ConcertoGraphics: Invalid present queue");
		return *_presentQueue;
	}

	RHI::FrameBuffer& VkRHISwapChain::GetCurrentFrameBuffer()
	{
		return *_frameBuffers[_currentFrameIndex];
	}

	const RHI::FrameBuffer& VkRHISwapChain::GetCurrentFrameBuffer() const
	{
		return *_frameBuffers[_currentFrameIndex];
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

	void VkRHISwapChain::CreateFrameBuffers(RHI::VkRHIDevice& device)
	{
		const std::span<Vk::ImageView> imagesViews = Vk::SwapChain::GetImageViews();
	
		_frameBuffers.reserve(imagesViews.size());
		for (const Vk::ImageView& imageView : imagesViews)
		{
			std::vector<std::unique_ptr<RHI::TextureView>> attachments;
			attachments.emplace_back(std::make_unique<VkRHITextureView>(imageView));
			attachments.emplace_back(std::make_unique<VkRHITextureView>(GetDepthImageView()));
			CONCERTO_ASSERT(imageView.Get() != VK_NULL_HANDLE && GetDepthImageView().Get() != VK_NULL_HANDLE, "ConcertoGraphics: iInvalid attachment");

			auto extent = Vk::SwapChain::GetExtent();
			auto fb = device.CreateFrameBuffer(extent.width, extent.height, Cast<VkRHIRenderPass&>(*_renderPass), attachments);
			CONCERTO_ASSERT(fb, "ConcertoGraphics: Could not create frame buffer");

			_frameBuffers.emplace_back(std::move(fb));
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
			for (UInt32 i = 0; i < imageCount; ++i)
			{
				_frames.emplace_back(*this);
			}
		}
	}

	VkRHISwapChain::SwapChainFrame::SwapChainFrame(VkRHISwapChain& owner) :
		_commandBuffer(owner.GetCommandPool().AllocateCommandBuffer(CommandBufferUasge::Primary)),
		_renderFence(*owner.GetDevice()),
		_presentSemaphore(*owner.GetDevice()),
		_renderSemaphore(*owner.GetDevice()),
		_owner(&owner),
		_imageIndex(0)
	{
	}

	void VkRHISwapChain::SwapChainFrame::Present()
	{
		const Vk::Queue& presentQueue = _owner->GetPresentQueue();

		presentQueue.Submit(Cast<VkRHICommandBuffer&>(*_commandBuffer), &_presentSemaphore, &_renderSemaphore, _renderFence);
		_owner->Present(_imageIndex);
	}

	RHI::CommandBuffer& VkRHISwapChain::SwapChainFrame::GetCommandBuffer()
	{
		return *_commandBuffer;
	}

	std::size_t VkRHISwapChain::SwapChainFrame::GetCurrentFrameIndex()
	{
		return _imageIndex;
	}

	RHI::FrameBuffer& VkRHISwapChain::SwapChainFrame::GetFrameBuffer()
	{
		return _owner->GetCurrentFrameBuffer();
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
