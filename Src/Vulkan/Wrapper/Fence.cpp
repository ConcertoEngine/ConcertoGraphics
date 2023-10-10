//
// Created by arthur on 15/06/22.
//

#include <cassert>
#include <stdexcept>

#include "Concerto/Graphics/Vulkan/Wrapper/Fence.hpp"
#include "Concerto/Graphics/Vulkan/Wrapper/Device.hpp"

namespace Concerto::Graphics
{
	Fence::Fence(Device& device, bool signaled) : Object<VkFence>(device, [this](Device &device, VkFence handle){
		vkDestroyFence(*device.Get(), handle, nullptr);
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

	void Fence::Wait(UInt64 timeout)
	{
		auto result = vkWaitForFences(*_device->Get(), 1, &_handle, true, timeout);
		if ( result != VK_SUCCESS)
		{
			throw std::runtime_error("vkWaitForFences fail error code : " + std::to_string(int(result)));
		}
	}

	void Fence::Reset()
	{
		if (vkResetFences(*_device->Get(), 1, &_handle) != VK_SUCCESS)
		{
			throw std::runtime_error("vkResetFences fail");
		}
	}
} // namespace Concerto::Graphics