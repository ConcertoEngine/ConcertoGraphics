//
// Created by arthur on 12/06/22.
//

#include <cassert>

#include "wrapper/Device.hpp"
#include "wrapper/FrameBuffer.hpp"
#include "wrapper/VulkanInitializer.hpp"

namespace Concerto::Graphics::Wrapper
{
	FrameBuffer::FrameBuffer(Device& device, Swapchain& swapchain, RenderPass& renderPass) : _frameBuffers(
			swapchain.GetImages().size()), _swapchain(swapchain), _renderPass(renderPass), _device(device)
	{
		VkFramebufferCreateInfo fb_info = VulkanInitializer::FramebufferCreateInfo(*renderPass.Get(),
				swapchain.GetExtent());

		for (int i = 0; i < swapchain.GetImages().size(); i++)
		{
			VkImageView attachments[2];
			attachments[0] = *swapchain.GetImageViews()[i].Get();
			attachments[1] = *swapchain.GetDepthImageView().Get();

			fb_info.pAttachments = attachments;
			fb_info.attachmentCount = 2;
			if(vkCreateFramebuffer(*_device.Get(), &fb_info, nullptr, &_frameBuffers[i]) != VK_SUCCESS)
			{
				throw std::runtime_error("failed to create framebuffer!");
			}
		}
	}

	FrameBuffer::~FrameBuffer()
	{
		for (VkFramebuffer _frameBuffer : _frameBuffers)
		{
			vkDestroyFramebuffer(*_device.Get(), _frameBuffer, nullptr);
		}
		_frameBuffers.clear();
	}

	VkFramebuffer FrameBuffer::operator[](std::size_t s)
	{
		assert(s < _frameBuffers.size());
		assert(_frameBuffers[s] != VK_NULL_HANDLE);
		return _frameBuffers[s];
	}
}