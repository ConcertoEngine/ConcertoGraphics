//
// Created by arthur on 14/06/22.
//


#include "wrapper/CommandPool.hpp"
#include <stdexcept>
#include <iostream>

namespace Concerto::Graphics::Wrapper
{
	CommandPool::CommandPool(VkDevice device, std::uint32_t queueFamily) : _device(device), _queueFamily(queueFamily)
	{
		VkCommandPoolCreateInfo info {};
		info.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
		info.pNext = nullptr;
		info.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
		info.queueFamilyIndex = queueFamily;

		VkResult result = vkCreateCommandPool(_device, &info, nullptr, &_commandPool);
		if (result != VK_SUCCESS)
		{
			throw std::runtime_error("Failed to create command pool");
		}
	}

	CommandPool::~CommandPool()
	{
		vkDestroyCommandPool(_device, _commandPool, nullptr);
		_commandPool = VK_NULL_HANDLE;
	}

	VkCommandPool CommandPool::get() const
	{
		return _commandPool;
	}

	void CommandPool::reset()
	{
		if (vkResetCommandPool(_device, _commandPool, 0) != VK_SUCCESS)
		{
			throw std::runtime_error("vkResetCommandPool fail");
		}
	}

}