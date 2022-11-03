//
// Created by arthu on 21/06/2022.
//

#ifndef CONCERTOGRAPHICS_ALLOCATOR_HPP
#define CONCERTOGRAPHICS_ALLOCATOR_HPP
#include "vulkan/vulkan.h"
#include "vk_mem_alloc.h"

namespace Concerto::Graphics::Wrapper
{
	class Instance
	class Allocator
	{
	public:
		Allocator(VkPhysicalDevice physicalDevice, VkDevice device, Instance& instance);
		~Allocator();

		VmaAllocator _allocator;
	};
}

#endif //CONCERTOGRAPHICS_ALLOCATOR_HPP
