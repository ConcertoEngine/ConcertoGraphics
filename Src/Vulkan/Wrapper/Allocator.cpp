//
// Created by arthur on 21/06/2022.
//

#include <cassert>
#include <stdexcept>

#include "Concerto/Graphics/Vulkan/wrapper/Allocator.hpp"
#include "Concerto/Graphics/Vulkan/wrapper/Instance.hpp"
#include "Concerto/Graphics/Vulkan/wrapper/PhysicalDevice.hpp"
#include "Concerto/Graphics/Vulkan/wrapper/Device.hpp"

namespace Concerto::Graphics
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