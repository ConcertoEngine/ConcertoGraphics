//
// Created by arthur on 15/06/22.
//

#include <cassert>
#include <stdexcept>

#include "Concerto/Graphics/Vulkan/Wrapper/Fence.hpp"

#include <Concerto/Core/Assert.hpp>

#include "Concerto/Graphics/Vulkan/Wrapper/Device.hpp"

namespace Concerto::Graphics
{
	Fence::Fence(Device& device, bool signaled) : Object(device)
	{
		VkFenceCreateInfo info = {};
		info.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
		info.pNext = nullptr;
		info.flags = signaled ? VK_FENCE_CREATE_SIGNALED_BIT : 0;
		_lastResult = vkCreateFence(*_device->Get(), &info, nullptr, &_handle);
		CONCERTO_ASSERT(_lastResult == VK_SUCCESS, "ConcertoGraphics: vkCreateFence failed VkResult={}", static_cast<int>(_lastResult));
	}

	Fence::~Fence()
	{
		if (IsNull())
			return;
		vkDestroyFence(*_device->Get(), _handle, nullptr);
	}

	void Fence::Wait(UInt64 timeout) const
	{
		const VkResult result = vkWaitForFences(*_device->Get(), 1, &_handle, true, timeout);
		CONCERTO_ASSERT(result == VK_SUCCESS, "ConcertoGraphics: vkWaitForFences failed VKResult={}", static_cast<int>(result));
	}

	void Fence::Reset() const
	{
		const VkResult result = vkResetFences(*_device->Get(), 1, &_handle);
		CONCERTO_ASSERT(result == VK_SUCCESS, "ConcertoGraphics: vkResetFences failed VKResult={}", static_cast<int>(result));
	}
} // namespace Concerto::Graphics