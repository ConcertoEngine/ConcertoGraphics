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
		SwapChain(pixelFormat, depthPixelFormat),
		Vk::Swapchain(device, window, Converters::ToVulkan(pixelFormat), Converters::ToVulkan(depthPixelFormat))
	{
		CreateRenderPass();
		CreateFrameBuffers();
	}

	void VkRHISwapChain::Present()
	{
		CONCERTO_ASSERT_FALSE("Not Implemented");
	}

	RHI::RenderPass& VkRHISwapChain::GetRenderPass()
	{
		CONCERTO_ASSERT(_renderPass, "ConcertoGraphics: Invalid renderpass");
		return *_renderPass;
	}

	Vector2u VkRHISwapChain::GetExtent()
	{
		return Vector2u{ Vk::Swapchain::GetExtent().width, Vk::Swapchain::GetExtent().height };
	}

	void VkRHISwapChain::CreateFrameBuffers()
	{
		const std::span<Vk::ImageView> imagesViews = Vk::Swapchain::GetImageViews();
		std::vector<VkImageView> vkImageViews;
		vkImageViews.reserve(imagesViews.size());
		for (auto& image: imagesViews)
			vkImageViews.push_back(*image.Get());

		_frameBuffers.reserve(vkImageViews.size());
		for (const VkImageView& imageView : vkImageViews)
		{
			std::vector attachments = {imageView, *GetDepthImageView().Get()};
			CONCERTO_ASSERT(attachments[0] != VK_NULL_HANDLE && attachments[1] != VK_NULL_HANDLE, "ConcertoGraphics: iInvalid attachment");

			Vk::FrameBuffer frameBuffer(*_device, static_cast<VkRHIRenderPass&>(*_renderPass), attachments, Vk::Swapchain::GetExtent());
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
}
