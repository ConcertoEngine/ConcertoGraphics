//
// Created by arthur on 15/06/22.
//

#include <cassert>
#include <stdexcept>

#include "Concerto/Graphics/Vulkan/Wrapper/Semaphore.hpp"

#include <Concerto/Core/Assert.hpp>

#include "Concerto/Graphics/Vulkan/Wrapper/Device.hpp"

namespace Concerto::Graphics
{

	Semaphore::Semaphore(Device& device) : Object(device)
	{
		VkSemaphoreCreateInfo info = {};
		info.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;
		info.pNext = nullptr;
		info.flags = 0;
		if (vkCreateSemaphore(*_device->Get(), &info, nullptr, &_handle) != VK_SUCCESS)
		{
			CONCERTO_ASSERT_FALSE;
			throw std::runtime_error("Failed to create semaphore");
		}
	}

	Semaphore::~Semaphore()
	{
		if (IsNull())
			return;
		vkDestroySemaphore(*_device->Get(), _handle, nullptr);
	}
} // namespace Concerto::Graphics
