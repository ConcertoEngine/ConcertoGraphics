//
// Created by arthur on 15/06/22.
//

#include <cassert>
#include <stdexcept>

#include "wrapper/Semaphore.hpp"

namespace Concerto::Graphics::Wrapper
{

	Semaphore::Semaphore(VkDevice device) : _device(device)
	{
		VkSemaphoreCreateInfo info = {};
		info.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;
		info.pNext = nullptr;
		info.flags = 0;
		if (vkCreateSemaphore(_device, &info, nullptr, &_semaphore) != VK_SUCCESS)
		{
			throw std::runtime_error("Failed to create semaphore");
		}
	}

	Semaphore::~Semaphore()
	{
		vkDestroySemaphore(_device, _semaphore, nullptr);
		_semaphore = VK_NULL_HANDLE;
	}

	VkSemaphore Semaphore::Get() const
	{
		assert(_semaphore != VK_NULL_HANDLE);
		return _semaphore;
	}
} // namespace Concerto::Graphics::Wrapper
