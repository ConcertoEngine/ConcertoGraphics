//
// Created by arthur on 15/05/2024.
//

#include <array>
#include <format>

#include <Concerto/Core/Cast.hpp>


#include "Concerto/Graphics/RHI/Vulkan/VkRHISwapChain.hpp"
#include "Concerto/Graphics/RHI/Vulkan/VkRHIDevice.hpp"
#include "Concerto/Graphics/RHI/Vulkan/Utils.hpp"
#include "Concerto/Graphics/RHI/Vulkan/VkRHIRenderPass.hpp"
#include "Concerto/Graphics/RHI/Vulkan/VkRHIDevice.hpp"
#include "Concerto/Graphics/RHI/Vulkan/VkRHICommandBuffer.hpp"
#include "Concerto/Graphics/RHI/Vulkan/VkRHICommandPool.hpp"
#include "Concerto/Graphics/RHI/Vulkan/VKRHITexture.hpp"

namespace cct::gfx::rhi
{
	VkRHISwapChain::VkRHISwapChain(rhi::VkRHIDevice& device, Window& window, PixelFormat pixelFormat, PixelFormat depthPixelFormat) :
		rhi::SwapChain(pixelFormat, depthPixelFormat),
		vk::SwapChain(device, window, Converters::ToVulkan(pixelFormat), Converters::ToVulkan(depthPixelFormat)),
		_pixelFormat(pixelFormat),
		_depthPixelFormat(depthPixelFormat)
	{
		CreateRenderPass();
		CreateFrameBuffers(device);
		_commandPool = device.CreateCommandPool(QueueFamily::Graphics);
		_presentQueue = std::make_unique<vk::Queue>(device, device.GetQueueFamilyIndex(vk::Queue::Type::Graphics));
		CreateFrames();
	}

	rhi::RenderPass& VkRHISwapChain::GetRenderPass()
	{
		CCT_ASSERT(_renderPass, "ConcertoGraphics: Invalid renderpass");
		return *_renderPass;
	}

	Vector2u VkRHISwapChain::GetExtent() const
	{
		return Vector2u{ vk::SwapChain::GetExtent().width, vk::SwapChain::GetExtent().height };
	}

	UInt32 VkRHISwapChain::GetImageCount() const
	{
		return static_cast<UInt32>(vk::SwapChain::GetImages().size());
	}

	Frame& VkRHISwapChain::AcquireFrame()
	{
		if (_needResize)
		{
			GetDevice()->WaitIdle();
			SwapChainFrame& frame = _frames[_currentFrameIndex];
			frame.Wait();
			vk::SwapChain::ReCreate(Converters::ToVulkan(_pixelFormat), Converters::ToVulkan(_depthPixelFormat));
			_frameBuffers.clear();
			_frames.clear();
			CreateRenderPass();
			CreateFrames();
			CreateFrameBuffers(GetRHIDevice());
			_needResize = false;
		}
		if (_currentFrameIndex != _lastFrameIndex)
		{
			SwapChainFrame& frame = _frames[_lastFrameIndex];
			frame.GetRenderFence().Wait(-1);
		}
		SwapChainFrame& currentFrame = _frames[_currentFrameIndex];
		const UInt32 nextImageIndex = vk::SwapChain::AcquireNextImage(currentFrame.GetPresentSemaphore(), nullptr);
		currentFrame.SetNextImageIndex(nextImageIndex);
		return currentFrame;
	}

	VkRHICommandPool& VkRHISwapChain::GetCommandPool() const
	{
		CCT_ASSERT(_commandPool, "ConcertoGraphics: Invalid command pool");
		return Cast<VkRHICommandPool&>(*_commandPool);
	}

	VkRHIDevice& VkRHISwapChain::GetRHIDevice() const
	{
		CCT_ASSERT(vk::SwapChain::GetDevice(), "ConcertoGraphics: Invalid device");
		return Cast<VkRHIDevice&>(*vk::SwapChain::GetDevice());
	}

	vk::Queue& VkRHISwapChain::GetPresentQueue() const
	{
		CCT_ASSERT(_presentQueue, "ConcertoGraphics: Invalid present queue");
		return *_presentQueue;
	}

	rhi::FrameBuffer& VkRHISwapChain::GetCurrentFrameBuffer()
	{
		return *_frameBuffers[_currentFrameIndex];
	}

	const rhi::FrameBuffer& VkRHISwapChain::GetCurrentFrameBuffer() const
	{
		return *_frameBuffers[_currentFrameIndex];
	}

	void VkRHISwapChain::Present(UInt32 imageIndex)
	{
		CCT_GFX_AUTO_PROFILER_SCOPE();

		_lastFrameIndex = _currentFrameIndex;
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
					CCT_ASSERT_FALSE("ConcertoGraphics: Present failed VKResult={}", static_cast<int>(_presentQueue->GetLastResult()));
				}
			}
		}
	}

	void VkRHISwapChain::CreateFrameBuffers(rhi::VkRHIDevice& device)
	{
		CCT_GFX_AUTO_PROFILER_SCOPE();

		const std::span<vk::ImageView> imagesViews = vk::SwapChain::GetImageViews();

		_frameBuffers.reserve(imagesViews.size());
		for (const vk::ImageView& imageView : imagesViews)
		{
			std::vector<std::unique_ptr<rhi::TextureView>> attachments;
			attachments.emplace_back(std::make_unique<VkRHITextureView>(imageView));
			attachments.emplace_back(std::make_unique<VkRHITextureView>(GetDepthImageView()));
			CCT_ASSERT(imageView.Get() != VK_NULL_HANDLE && GetDepthImageView().Get() != VK_NULL_HANDLE, "ConcertoGraphics: iInvalid attachment");

			auto extent = vk::SwapChain::GetExtent();
			auto fb = device.CreateFrameBuffer(extent.width, extent.height, Cast<VkRHIRenderPass&>(*_renderPass), attachments);
			CCT_ASSERT(fb, "ConcertoGraphics: Could not create frame buffer");

			_frameBuffers.emplace_back(std::move(fb));
		}
	}

	void VkRHISwapChain::CreateRenderPass()
	{
		CCT_GFX_AUTO_PROFILER_SCOPE();

		std::vector<rhi::RenderPass::Attachment> attachment;
		auto& colorAttachment = attachment.emplace_back();
		colorAttachment.pixelFormat = GetPixelFormat();
		colorAttachment.loadOp = rhi::AttachmentLoadOp::Clear;
		colorAttachment.storeOp = rhi::AttachmentStoreOp::Store;
		colorAttachment.stencilLoadOp = rhi::AttachmentLoadOp::DontCare;
		colorAttachment.stencilStoreOp = rhi::AttachmentStoreOp::DontCare;
		colorAttachment.initialLayout = rhi::ImageLayout::Undefined;
		colorAttachment.finalLayout = rhi::ImageLayout::PresentSrcKhr;

		auto& depthAttachment = attachment.emplace_back();
		depthAttachment.pixelFormat = GetDepthPixelFormat();
		depthAttachment.loadOp = rhi::AttachmentLoadOp::Clear;
		depthAttachment.storeOp = rhi::AttachmentStoreOp::Store;
		depthAttachment.stencilLoadOp = rhi::AttachmentLoadOp::Clear;
		depthAttachment.stencilStoreOp = rhi::AttachmentStoreOp::DontCare;
		depthAttachment.initialLayout = rhi::ImageLayout::Undefined;
		depthAttachment.finalLayout = rhi::ImageLayout::DepthStencilAttachmentOptimal;

		std::vector<rhi::RenderPass::SubPassDescription> subPassDescriptions;

		auto& subPass = subPassDescriptions.emplace_back();
		subPass.colorAttachments.push_back({ 0, rhi::ImageLayout::ColorAttachmentOptimal });
		subPass.depthStencilAttachment = { 1, rhi::ImageLayout::DepthStencilAttachmentOptimal };


		std::vector<rhi::RenderPass::SubPassDependency> subPassDependencies;
		auto& colorDependency = subPassDependencies.emplace_back();
		colorDependency.srcSubPassIndex = rhi::RenderPass::SubPassDependency::ExternalSubPass;
		colorDependency.dstSubPassIndex = 0u;
		colorDependency.srcStageMask = static_cast<rhi::PipelineStageFlags>(rhi::PipelineStage::ColorAttachmentOutput);
		colorDependency.srcAccessFlags = 0u;
		colorDependency.dstStageMask = static_cast<rhi::PipelineStageFlags>(rhi::PipelineStage::ColorAttachmentOutput);
		colorDependency.dstAccessFlags = static_cast<rhi::MemoryAccessFlags>(rhi::MemoryAccess::ColorAttachmentWrite);

		auto& depthDependency = subPassDependencies.emplace_back();
		depthDependency.srcSubPassIndex = rhi::RenderPass::SubPassDependency::ExternalSubPass;
		depthDependency.dstSubPassIndex = 0u;
		depthDependency.srcStageMask = static_cast<rhi::PipelineStageFlags>(rhi::PipelineStage::EarlyFragmentTests) | static_cast<rhi::PipelineStageFlags>(rhi::PipelineStage::LateFragmentTests);
		depthDependency.srcAccessFlags = 0u;
		depthDependency.dstStageMask = static_cast<rhi::PipelineStageFlags>(rhi::PipelineStage::EarlyFragmentTests) | static_cast<rhi::PipelineStageFlags>(rhi::PipelineStage::LateFragmentTests);
		depthDependency.dstAccessFlags = static_cast<rhi::MemoryAccessFlags>(rhi::MemoryAccess::DepthStencilAttachmentWrite);

		_renderPass = Cast<VkRHIDevice&>(*_device).CreateRenderPass(attachment, subPassDescriptions, subPassDependencies);
		CCT_ASSERT(_renderPass && Cast<VkRHIRenderPass&>(*_renderPass).GetLastResult() == VK_SUCCESS, "ConcertoGraphics: Could not create render pass");
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
#ifdef CCT_DEBUG
		Cast<VkRHICommandBuffer&>(*_commandBuffer).SetDebugName("SwapChainFrameCommandBuffer");
		_renderFence.SetDebugName("SwapChainFrameRenderFence");
		_presentSemaphore.SetDebugName("SwapChainFramePresentSemaphore");
		_renderSemaphore.SetDebugName("SwapChainFrameRenderSemaphore");
#endif
	}

	void VkRHISwapChain::SwapChainFrame::Present()
	{
		CCT_GFX_AUTO_PROFILER_SCOPE();

		const vk::Queue& presentQueue = _owner->GetPresentQueue();

		_renderFence.Wait(-1);
		_renderFence.Reset();
		presentQueue.Submit(Cast<VkRHICommandBuffer&>(*_commandBuffer), &_presentSemaphore, &_renderSemaphore, _renderFence);
		_owner->Present(_imageIndex);
		CCT_GFX_FRAME_MARK;
	}

	rhi::CommandBuffer& VkRHISwapChain::SwapChainFrame::GetCommandBuffer()
	{
		return *_commandBuffer;
	}

	std::size_t VkRHISwapChain::SwapChainFrame::GetCurrentFrameIndex()
	{
		return _imageIndex;
	}

	rhi::FrameBuffer& VkRHISwapChain::SwapChainFrame::GetFrameBuffer()
	{
		return _owner->GetCurrentFrameBuffer();
	}

	void VkRHISwapChain::SwapChainFrame::SetNextImageIndex(UInt32 imageIndex)
	{
		_imageIndex = imageIndex;
#ifdef CCT_DEBUG
		Cast<VkRHICommandBuffer&>(*_commandBuffer).SetDebugName(std::format("SwapChainFrameCommandBuffer[{}]", imageIndex));
		_renderFence.SetDebugName(std::format("SwapChainFrameRenderFence {}", imageIndex));
		_presentSemaphore.SetDebugName(std::format("SwapChainFramePresentSemaphore {}", imageIndex));
		_renderSemaphore.SetDebugName(std::format("SwapChainFrameRenderSemaphore {}", imageIndex));
#endif
	}

	void VkRHISwapChain::SwapChainFrame::Wait()
	{
		CCT_GFX_AUTO_PROFILER_SCOPE();

		_renderFence.Wait(-1);
		_renderFence.Reset();
	}

	vk::Semaphore& VkRHISwapChain::SwapChainFrame::GetPresentSemaphore()
	{
		return _presentSemaphore;
	}

	vk::Semaphore& VkRHISwapChain::SwapChainFrame::GetRenderSemaphore()
	{
		return _renderSemaphore;
	}

	vk::Fence& VkRHISwapChain::SwapChainFrame::GetRenderFence()
	{
		return _renderFence;
	}
}
