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
		_lastResult = _device->vkCreateFence(*_device->Get(), &info, nullptr, &_handle);
		CCT_ASSERT(_lastResult == VK_SUCCESS, "ConcertoGraphics: vkCreateFence failed VkResult={}", static_cast<int>(_lastResult));
	}

	Fence::~Fence()
	{
		if (IsNull())
			return;
		_device->vkDestroyFence(*_device->Get(), _handle, nullptr);
	}

	void Fence::Wait(UInt64 timeout) const
	{
		CCT_GFX_AUTO_PROFILER_SCOPE();

		const VkResult result = _device->vkWaitForFences(*_device->Get(), 1, &_handle, true, timeout);
		CCT_ASSERT(result == VK_SUCCESS, "ConcertoGraphics: vkWaitForFences failed VKResult={}", static_cast<int>(result));
	}

	void Fence::Reset() const
	{
		const VkResult result = _device->vkResetFences(*_device->Get(), 1, &_handle);
		CCT_ASSERT(result == VK_SUCCESS, "ConcertoGraphics: vkResetFences failed VKResult={}", static_cast<int>(result));
	}
} // namespace cct::gfx::vk