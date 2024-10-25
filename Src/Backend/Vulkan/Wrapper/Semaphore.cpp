//
// Created by arthur on 15/06/22.
//

#include <stdexcept>

#include <Concerto/Core/Assert.hpp>

#include "Concerto/Graphics/Backend/Vulkan/Wrapper/Semaphore.hpp"
#include "Concerto/Graphics/Backend/Vulkan/Wrapper/Device.hpp"

namespace Concerto::Graphics::Vk
{

	Semaphore::Semaphore(Device& device) : Object(device)
	{
		VkSemaphoreCreateInfo info = {};
		info.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;
		info.pNext = nullptr;
		info.flags = 0;
		_lastResult = _device->vkCreateSemaphore(*_device->Get(), &info, nullptr, &_handle);
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
		_device->vkDestroySemaphore(*_device->Get(), _handle, nullptr);
	}
} // namespace Concerto::Graphics::Vk
