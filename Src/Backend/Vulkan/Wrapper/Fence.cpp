//
// Created by arthur on 15/06/22.
//

#include <cassert>
#include <stdexcept>

#include "Concerto/Graphics/Backend/Vulkan/Wrapper/Fence.hpp"

#include <Concerto/Core/Assert.hpp>

#include "Concerto/Graphics/Backend/Vulkan/Wrapper/Device.hpp"

namespace cct::gfx::vk
{
	Fence::Fence(Device& device, bool signaled) : Object(device)
	{
		VkFenceCreateInfo info = {};
		info.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
		info.pNext = nullptr;
		info.flags = signaled ? VK_FENCE_CREATE_SIGNALED_BIT : 0;
		m_lastResult = m_device->vkCreateFence(*m_device->Get(), &info, nullptr, &m_handle);
		CCT_ASSERT(m_lastResult == VK_SUCCESS, "ConcertoGraphics: vkCreateFence failed VkResult={}", static_cast<int>(m_lastResult));
	}

	Fence::~Fence()
	{
		if (IsNull())
			return;
		m_device->vkDestroyFence(*m_device->Get(), m_handle, nullptr);
	}

	void Fence::Wait(UInt64 timeout) const
	{
		CCT_GFX_AUTO_PROFILER_SCOPE();

		const VkResult result = m_device->vkWaitForFences(*m_device->Get(), 1, &m_handle, true, timeout);
		CCT_ASSERT(result == VK_SUCCESS, "ConcertoGraphics: vkWaitForFences failed VKResult={}", static_cast<int>(result));
	}

	void Fence::Reset() const
	{
		const VkResult result = m_device->vkResetFences(*m_device->Get(), 1, &m_handle);
		CCT_ASSERT(result == VK_SUCCESS, "ConcertoGraphics: vkResetFences failed VKResult={}", static_cast<int>(result));
	}
} // namespace cct::gfx::vk