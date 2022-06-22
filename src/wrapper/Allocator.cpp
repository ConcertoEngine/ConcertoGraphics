//
// Created by arthu on 21/06/2022.
//

#include "wrapper/Allocator.hpp"
#include <assert.h>
#include <stdexcept>
namespace Concerto::Graphics::Wrapper
{
	Allocator::Allocator(VkPhysicalDevice physicalDevice, VkDevice device, VkInstance instance) : _allocator(VK_NULL_HANDLE)
	{
		VmaAllocatorCreateInfo allocatorInfo = {};
		allocatorInfo.physicalDevice = physicalDevice;
		allocatorInfo.device = device;
		allocatorInfo.instance = instance;
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