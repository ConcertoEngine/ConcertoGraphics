//
// Created by arthur on 12/06/22.
//


#include "wrapper/FrameBuffer.hpp"
#include "wrapper/VulkanInitializer.hpp"
namespace Concerto::Graphics::Wrapper
{
	FrameBuffer::FrameBuffer(VkDevice device, Swapchain& swapchain, RenderPass& renderPass) : _frameBuffers(swapchain.getImageCount()), _swapchain(swapchain), _renderPass(renderPass), _device(device)
	{
		VkFramebufferCreateInfo fb_info = VulkanInitializer::FramebufferCreateInfo(renderPass.getRenderPass(), swapchain.getExtent());

		for (int i = 0; i < swapchain.getImageCount(); i++)
		{

			VkImageView attachments[2];
			attachments[0] = swapchain.getImageViews()[i];
			attachments[1] = swapchain.getDepthImageView();

			fb_info.pAttachments = attachments;
			fb_info.attachmentCount = 2;
			if(vkCreateFramebuffer(device, &fb_info, nullptr, &_frameBuffers[i]) != VK_SUCCESS)
			{
				throw std::runtime_error("failed to create framebuffer!");
			}
		}
	}

	FrameBuffer::~FrameBuffer()
	{
		for (int i = 0; i < _frameBuffers.size(); i++)
		{
			vkDestroyFramebuffer(_device, _frameBuffers[i], nullptr);
			vkDestroyImageView(_device, _swapchain.getImageViews()[i], nullptr);
		}
	}
}