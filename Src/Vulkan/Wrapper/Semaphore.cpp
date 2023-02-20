//
// Created by arthur on 15/06/22.
//

#include <cassert>
#include <stdexcept>

#include "Vulkan/wrapper/Semaphore.hpp"
#include "Vulkan/wrapper/Device.hpp"

namespace Concerto::Graphics::Wrapper
{

	Semaphore::Semaphore(Device& device) : Object<VkSemaphore>(device, [](Device &device, VkSemaphore handle)
	{ vkDestroySemaphore(*device.Get(), handle, nullptr); })
	{
		VkSemaphoreCreateInfo info = {};
		info.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;
		info.pNext = nullptr;
		info.flags = 0;
		if (vkCreateSemaphore(*_device->Get(), &info, nullptr, &_handle) != VK_SUCCESS)
		{
			throw std::runtime_error("Failed to create semaphore");
		}
	}
} // namespace Concerto::Graphics::Wrapper
