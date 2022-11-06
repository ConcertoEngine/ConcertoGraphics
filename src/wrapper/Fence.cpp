//
// Created by arthur on 15/06/22.
//

#include <cassert>
#include <stdexcept>

#include "wrapper/Fence.hpp"
#include "wrapper/Device.hpp"

namespace Concerto::Graphics::Wrapper
{
	Fence::Fence(Device& device, bool signaled) : Object<VkFence>(device, [this](){
		vkDestroyFence(*_device->Get(), _handle, nullptr);
	})
	{
		VkFenceCreateInfo info = {};
		info.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
		info.pNext = nullptr;
		info.flags = signaled ? VK_FENCE_CREATE_SIGNALED_BIT : 0;
		if (vkCreateFence(*_device->Get(), &info, nullptr, &_handle) != VK_SUCCESS)
		{
			throw std::runtime_error("Failed to create fence");
		}
	}

	void Fence::wait(std::uint64_t timeout)
	{
		if (vkWaitForFences(*_device->Get(), 1, &_handle, true, timeout) != VK_SUCCESS)
		{
			throw std::runtime_error("vkWaitForFences fail");
		}
	}

	void Fence::reset()
	{
		if (vkResetFences(*_device->Get(), 1, &_handle) != VK_SUCCESS)
		{
			throw std::runtime_error("vkResetFences fail");
		}
	}
} // namespace Concerto::Graphics::Wrapper