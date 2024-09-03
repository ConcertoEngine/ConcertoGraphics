//
// Created by arthur on 14/06/22.
//

#include <stdexcept>
#include <iostream>

#include <Concerto/Core/Assert.hpp>

#include "Concerto/Graphics/Backend/Vulkan/Wrapper/CommandPool.hpp"
#include "Concerto/Graphics/Backend/Vulkan/Wrapper/Device.hpp"

namespace Concerto::Graphics::Vk
{
	CommandPool::CommandPool(Device& device, UInt32 queueFamily) :
		Object(device),
		_queueFamily(queueFamily)
	{
		VkCommandPoolCreateInfo info{};
		info.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
		info.pNext = nullptr;
		info.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
		info.queueFamilyIndex = _queueFamily;
		_lastResult = vkCreateCommandPool(*_device->Get(), &info, nullptr, &_handle);
		CONCERTO_ASSERT(_lastResult == VK_SUCCESS, "Error cannot create command pool: VkResult={}", static_cast<int>(_lastResult));
	}

	CommandPool::~CommandPool()
	{
		if (IsNull())
			return;
		vkDestroyCommandPool(*_device->Get(), _handle, nullptr);
	}

	void CommandPool::Reset() const
	{
		_lastResult = vkResetCommandPool(*_device->Get(), _handle, 0);
		CONCERTO_ASSERT(_lastResult == VK_SUCCESS, "ConcertoGraphics: Error cannot reset command pool: VkResult={}", static_cast<int>(_lastResult));
	}
}