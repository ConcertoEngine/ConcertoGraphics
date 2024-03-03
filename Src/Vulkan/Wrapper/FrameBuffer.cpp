//
// Created by arthur on 12/06/22.
//

#include <cassert>

#include "Concerto/Graphics/Vulkan/Wrapper/Device.hpp"
#include "Concerto/Graphics/Vulkan/Wrapper/FrameBuffer.hpp"

#include <Concerto/Core/Assert.hpp>

#include "Concerto/Graphics/Vulkan/Wrapper/VulkanInitializer.hpp"
#include "Concerto/Graphics/Vulkan/Wrapper/RenderPass.hpp"
#include "Concerto/Graphics/Vulkan/Wrapper/ImageView.hpp"

namespace Concerto::Graphics
{
	FrameBuffer::FrameBuffer(Device& device, RenderPass& renderPass, ImageView& imageView, ImageView& depthImageView, VkExtent2D extent) : Object(device)
	{
		VkFramebufferCreateInfo framebufferCreateInfo = VulkanInitializer::FramebufferCreateInfo(*renderPass.Get(), extent);

		VkImageView attachments[2];
		attachments[0] = *imageView.Get();
		attachments[1] = *depthImageView.Get();

		framebufferCreateInfo.pAttachments = attachments;
		framebufferCreateInfo.attachmentCount = 2;
		_lastResult = vkCreateFramebuffer(*_device->Get(), &framebufferCreateInfo, nullptr, &_handle);
		CONCERTO_ASSERT(_lastResult == VK_SUCCESS, "ConcertoGraphics: vkCreateFramebuffer failed VkResult={}", static_cast<int>(_lastResult));
	}

	FrameBuffer::~FrameBuffer()
	{
		if (IsNull())
			return;
		vkDestroyFramebuffer(*_device->Get(), _handle, nullptr);
	}
} // Concerto::Graphics::Wrapper