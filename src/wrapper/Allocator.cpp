//
// Created by arthur on 21/06/2022.
//

#include <cassert>
#include <stdexcept>
#include "wrapper/Allocator.hpp"
#include "wrapper/Instance.hpp"
#include "wrapper/PhysicalDevice.hpp"
#include "wrapper/Device.hpp"

namespace Concerto::Graphics::Wrapper
{
	Allocator::Allocator(PhysicalDevice& physicalDevice, Device& device, Instance& instance) : Object<VmaAllocator>(
			device, [](Device &device, VmaAllocator handle)
			{
				vmaDestroyAllocator(handle);
			}), _device(&device)
	{
		VmaAllocatorCreateInfo allocatorInfo = {};
		allocatorInfo.physicalDevice = *physicalDevice.Get();
		allocatorInfo.device = *device.Get();
		allocatorInfo.instance = *instance.Get();
		if (vmaCreateAllocator(&allocatorInfo, &_handle) != VK_SUCCESS)
		{
			throw std::runtime_error("VMA : Unable to create allocator");
		}
	}

	Device& Allocator::GetDevice() const
	{
		assert(_device != nullptr);
		return *_device;
	}
}