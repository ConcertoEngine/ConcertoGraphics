//
// Created by arthur on 12/06/22.
//

#include <Concerto/Core/Assert.hpp>

#include "Concerto/Graphics/Backend/Vulkan/Wrapper/Device.hpp"
#include "Concerto/Graphics/Backend/Vulkan/Wrapper/FrameBuffer.hpp"
#include "Concerto/Graphics/Backend/Vulkan/Wrapper/VulkanInitializer.hpp"
#include "Concerto/Graphics/Backend/Vulkan/Wrapper/RenderPass.hpp"
#include "Concerto/Graphics/Backend/Vulkan/Wrapper/ImageView.hpp"

namespace cct::gfx::vk
{
	FrameBuffer::FrameBuffer(Device& device, const RenderPass& renderPass, const std::vector<VkImageView>& attachments, VkExtent2D extent) :
		Object(device),
		m_extent(extent)
	{
		VkFramebufferCreateInfo frameBufferCreateInfo = VulkanInitializer::FramebufferCreateInfo(*renderPass.Get(), extent);

		frameBufferCreateInfo.pAttachments = attachments.data();
		frameBufferCreateInfo.attachmentCount = static_cast<UInt32>(attachments.size());
		m_lastResult = m_device->vkCreateFramebuffer(*m_device->Get(), &frameBufferCreateInfo, nullptr, &m_handle);
		CCT_ASSERT(m_lastResult == VK_SUCCESS, "ConcertoGraphics: vkCreateFramebuffer failed VkResult={}", static_cast<int>(m_lastResult));
	}

	FrameBuffer::~FrameBuffer()
	{
		if (IsNull())
			return;
		m_device->vkDestroyFramebuffer(*m_device->Get(), m_handle, nullptr);
	}

	VkExtent2D FrameBuffer::GetExtent2D() const
	{
		return m_extent;
	}
} // cct::gfx::vk