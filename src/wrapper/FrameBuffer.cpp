//
// Created by arthur on 12/06/22.
//

#include <cassert>

#include "wrapper/Device.hpp"
#include "wrapper/FrameBuffer.hpp"
#include "wrapper/VulkanInitializer.hpp"
#include "wrapper/RenderPass.hpp"
#include "wrapper/ImageView.hpp"

namespace Concerto::Graphics::Wrapper
{
	FrameBuffer::FrameBuffer(Device& device, RenderPass& renderPass, ImageView& imageView, ImageView& depthImageView,
			VkExtent2D extent) : Object<VkFramebuffer>(device, [](Device &device, VkFramebuffer handle)
	{
				vkDestroyFramebuffer(*device.Get(), handle, nullptr);
	})
	{
		VkFramebufferCreateInfo fb_info = VulkanInitializer::FramebufferCreateInfo(*renderPass.Get(), extent);

		VkImageView attachments[2];
		attachments[0] = *imageView.Get();
		attachments[1] = *depthImageView.Get();

		fb_info.pAttachments = attachments;
		fb_info.attachmentCount = 2;
		if (vkCreateFramebuffer(*_device->Get(), &fb_info, nullptr, &_handle) != VK_SUCCESS)
		{
			throw std::runtime_error("failed to create framebuffer!");
		}

	}
} // Concerto::Graphics::Wrapper