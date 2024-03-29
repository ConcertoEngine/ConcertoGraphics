//
// Created by arthur on 12/06/22.
//


#include <stdexcept>
#include <cassert>

#include "Concerto/Graphics/Vulkan/Wrapper/RenderPass.hpp"

#include <Concerto/Core/Assert.hpp>

#include "Concerto/Graphics/Vulkan/Wrapper/Swapchain.hpp"
#include "Concerto/Graphics/Vulkan/Wrapper/Device.hpp"

namespace Concerto::Graphics
{
	RenderPass::RenderPass(Device& device, Swapchain& swapchain) :
		Object(device),
		_swapchain(swapchain)
	{
		VkAttachmentDescription color_attachment = {};
		color_attachment.format = swapchain.GetImageFormat();
		color_attachment.samples = VK_SAMPLE_COUNT_1_BIT;
		color_attachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
		color_attachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
		color_attachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
		color_attachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
		color_attachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
		color_attachment.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

		VkAttachmentReference color_attachment_ref = {};
		color_attachment_ref.attachment = 0;
		color_attachment_ref.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

		VkAttachmentDescription depth_attachment = {};
		// Depth attachment
		depth_attachment.flags = 0;
		depth_attachment.format = swapchain.GetDepthFormat();
		depth_attachment.samples = VK_SAMPLE_COUNT_1_BIT;
		depth_attachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
		depth_attachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
		depth_attachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
		depth_attachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
		depth_attachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
		depth_attachment.finalLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

		VkAttachmentReference depth_attachment_ref = {};
		depth_attachment_ref.attachment = 1;
		depth_attachment_ref.layout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

		//we are going to create 1 subpass, which is the minimum you can do
		VkSubpassDescription subpass = {};
		subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
		subpass.colorAttachmentCount = 1;
		subpass.pColorAttachments = &color_attachment_ref;
		//hook the depth attachment into the subpass
		subpass.pDepthStencilAttachment = &depth_attachment_ref;

		//1 dependency, which is from "outside" into the subpass. And we can read or write color
		VkSubpassDependency dependency = {};
		dependency.srcSubpass = VK_SUBPASS_EXTERNAL;
		dependency.dstSubpass = 0;
		dependency.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
		dependency.srcAccessMask = 0;
		dependency.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
		dependency.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;

		//dependency from outside to the subpass, making this subpass dependent on the previous renderpasses
		VkSubpassDependency depth_dependency = {};
		depth_dependency.srcSubpass = VK_SUBPASS_EXTERNAL;
		depth_dependency.dstSubpass = 0;
		depth_dependency.srcStageMask =
				VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT | VK_PIPELINE_STAGE_LATE_FRAGMENT_TESTS_BIT;
		depth_dependency.srcAccessMask = 0;
		depth_dependency.dstStageMask =
				VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT | VK_PIPELINE_STAGE_LATE_FRAGMENT_TESTS_BIT;
		depth_dependency.dstAccessMask = VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;

		VkAttachmentDescription attachments[] = { color_attachment, depth_attachment };
		VkRenderPassCreateInfo render_pass_info = {};
		render_pass_info.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
		render_pass_info.attachmentCount = 2;
		render_pass_info.pAttachments = attachments;
		render_pass_info.subpassCount = 1;
		render_pass_info.pSubpasses = &subpass;
		render_pass_info.dependencyCount = 1;
		render_pass_info.pDependencies = &depth_dependency;

		const VkResult result = vkCreateRenderPass(*_device->Get(), &render_pass_info, nullptr, &_handle);
		CONCERTO_ASSERT(result == VK_SUCCESS, "ConcertoGraphics: vkCreateRenderPass failed VKResult={}", static_cast<int>(result));
	}

	RenderPass::~RenderPass()
	{
		if (IsNull())
			return;
		vkDestroyRenderPass(*_device->Get(), _handle, nullptr);
	}

	VkExtent2D RenderPass::GetWindowExtent() const
	{
		return _swapchain.GetExtent();
	}
} // Concerto::Graphics::Wrapper
