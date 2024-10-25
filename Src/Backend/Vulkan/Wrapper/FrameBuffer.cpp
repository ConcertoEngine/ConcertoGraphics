//
// Created by arthur on 12/06/22.
//

#include <Concerto/Core/Assert.hpp>

#include "Concerto/Graphics/Backend/Vulkan/Wrapper/Device.hpp"
#include "Concerto/Graphics/Backend/Vulkan/Wrapper/FrameBuffer.hpp"
#include "Concerto/Graphics/Backend/Vulkan/Wrapper/VulkanInitializer.hpp"
#include "Concerto/Graphics/Backend/Vulkan/Wrapper/RenderPass.hpp"
#include "Concerto/Graphics/Backend/Vulkan/Wrapper/ImageView.hpp"

namespace Concerto::Graphics::Vk
{
	FrameBuffer::FrameBuffer(Device& device, const RenderPass& renderPass, const std::vector<VkImageView>& attachments, VkExtent2D extent) :
		Object(device),
		_extent(extent)
	{
		VkFramebufferCreateInfo frameBufferCreateInfo = VulkanInitializer::FramebufferCreateInfo(*renderPass.Get(), extent);

		frameBufferCreateInfo.pAttachments = attachments.data();
		frameBufferCreateInfo.attachmentCount = static_cast<UInt32>(attachments.size());
		_lastResult = _device->vkCreateFramebuffer(*_device->Get(), &frameBufferCreateInfo, nullptr, &_handle);
		CONCERTO_ASSERT(_lastResult == VK_SUCCESS, "ConcertoGraphics: vkCreateFramebuffer failed VkResult={}", static_cast<int>(_lastResult));
	}

	FrameBuffer::~FrameBuffer()
	{
		if (IsNull())
			return;
		_device->vkDestroyFramebuffer(*_device->Get(), _handle, nullptr);
	}

	VkExtent2D FrameBuffer::GetExtent2D() const
	{
		return _extent;
	}
} // Concerto::Graphics::Vk