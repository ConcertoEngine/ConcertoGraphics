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
		_lastResult = vkCreateSemaphore(*_device->Get(), &info, nullptr, &_handle);
		if (_lastResult != VK_SUCCESS)
		{
			CONCERTO_ASSERT_FALSE("ConcertoGraphics: vkCreateSemaphore failed VKResult={}", static_cast<int>(_lastResult));
			return;
		}
	}

	Semaphore::~Semaphore()
	{
		if (IsNull())
			return;
		vkDestroySemaphore(*_device->Get(), _handle, nullptr);
	}
} // namespace Concerto::Graphics
