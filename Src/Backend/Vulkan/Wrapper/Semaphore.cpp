//
// Created by arthur on 15/06/22.
//

#include <stdexcept>

#include <Concerto/Core/Assert.hpp>

#include "Concerto/Graphics/Backend/Vulkan/Wrapper/Semaphore.hpp"
#include "Concerto/Graphics/Backend/Vulkan/Wrapper/Device.hpp"

namespace cct::gfx::vk
{

	Semaphore::Semaphore(Device& device) : Object(device)
	{
		VkSemaphoreCreateInfo info = {};
		info.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;
		info.pNext = nullptr;
		info.flags = 0;
		m_lastResult = m_device->vkCreateSemaphore(*m_device->Get(), &info, nullptr, &m_handle);
		if (m_lastResult != VK_SUCCESS)
		{
			CCT_ASSERT_FALSE("ConcertoGraphics: vkCreateSemaphore failed VKResult={}", static_cast<int>(m_lastResult));
			return;
		}
	}

	Semaphore::~Semaphore()
	{
		if (IsNull())
			return;
		m_device->vkDestroySemaphore(*m_device->Get(), m_handle, nullptr);
	}
} // namespace cct::gfx::vk
