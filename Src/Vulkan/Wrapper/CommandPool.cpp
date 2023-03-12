//
// Created by arthur on 14/06/22.
//

#include <cassert>
#include <stdexcept>
#include <iostream>

#include "Vulkan/wrapper/CommandPool.hpp"
#include "Vulkan/wrapper/Device.hpp"

namespace Concerto::Graphics::Wrapper
{
	CommandPool::CommandPool(Device& device, std::uint32_t queueFamily) : Object<VkCommandPool>(device,
			[](Device &device, VkCommandPool handle)
			{
				vkDestroyCommandPool(*device.Get(), handle, nullptr);
			}), _queueFamily(queueFamily)
	{
		VkCommandPoolCreateInfo info{};
		info.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
		info.pNext = nullptr;
		info.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
		info.queueFamilyIndex = _queueFamily;

		VkResult result = vkCreateCommandPool(*_device->Get(), &info, nullptr, &_handle);
		if (result != VK_SUCCESS)
		{
			throw std::runtime_error("Failed to create command pool");
		}
	}

	void CommandPool::Reset()
	{
		if (vkResetCommandPool(*_device->Get(), _handle, 0) != VK_SUCCESS)
		{
			throw std::runtime_error("vkResetCommandPool fail");
		}
	}

}