//
// Created by arthur on 21/06/2022.
//

#include <cassert>
#include <stdexcept>
#include "wrapper/Allocator.hpp"
#include "wrapper/Instance.hpp"
namespace Concerto::Graphics::Wrapper
{
	Allocator::Allocator(VkPhysicalDevice physicalDevice, VkDevice device, Instance& instance) : _allocator(VK_NULL_HANDLE)
	{
		VmaAllocatorCreateInfo allocatorInfo = {};
		allocatorInfo.physicalDevice = physicalDevice;
		allocatorInfo.device = device;
		allocatorInfo.instance = *instance.Get();
		if (vmaCreateAllocator(&allocatorInfo, &_allocator) != VK_SUCCESS)
		{
			throw std::runtime_error("VMA : Unable to create allocator");
		}
		assert(_allocator);
	}

	Allocator::~Allocator()
	{
		vmaDestroyAllocator(_allocator);
	}
}