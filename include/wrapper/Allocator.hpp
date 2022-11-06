//
// Created by arthu on 21/06/2022.
//

#ifndef CONCERTOGRAPHICS_ALLOCATOR_HPP
#define CONCERTOGRAPHICS_ALLOCATOR_HPP

#include "vulkan/vulkan.h"
#include "vk_mem_alloc.h"
#include "wrapper/Object.hpp"

namespace Concerto::Graphics::Wrapper
{
	class Instance;

	class PhysicalDevice;

	class Device;

	class Allocator : public Object<VmaAllocator>
	{
	public:
		Allocator(PhysicalDevice& physicalDevice, Device& device, Instance& instance);

		Allocator(const Allocator&) = delete;

		Allocator(Allocator&&) = default;

		Allocator& operator=(const Allocator&) = delete;

		Allocator& operator=(Allocator&&) = default;
	};
}

#endif //CONCERTOGRAPHICS_ALLOCATOR_HPP
