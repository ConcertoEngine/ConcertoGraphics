//
// Created by arthur on 09/06/22.
//

#ifndef CONCERTOGRAPHICS_ALLOCATEDBUFFER_HPP
#define CONCERTOGRAPHICS_ALLOCATEDBUFFER_HPP

#include <vulkan/vulkan.h>
#include <cstddef>
#include "vk_mem_alloc.h"
#include "Allocator.hpp"
#include "Fence.hpp"
#include "CommandPool.hpp"

namespace Concerto::Graphics::Wrapper
{
	class AllocatedBuffer
	{
	public:
		AllocatedBuffer(Allocator& allocator, std::size_t allocSize,
				VkBufferUsageFlags usage, VmaMemoryUsage memoryUsage);

		AllocatedBuffer(AllocatedBuffer&&) = default;

		AllocatedBuffer(const AllocatedBuffer&) = delete;

		AllocatedBuffer& operator=(AllocatedBuffer&&) = delete;

		AllocatedBuffer& operator=(const AllocatedBuffer&) = delete;

		~AllocatedBuffer();

		Allocator &_allocator;
		VkBuffer _buffer {VK_NULL_HANDLE};
		VmaAllocation _allocation {VK_NULL_HANDLE};
	};

	template<typename T>
	AllocatedBuffer
	MakeAllocatedBuffer(Allocator& allocator, VkBufferUsageFlags usage,
			VmaMemoryUsage memoryUsage)
	{
		return { allocator, sizeof(T), usage, memoryUsage };
	}

	template<typename T>
	AllocatedBuffer
	MakeAllocatedBuffer(Allocator& allocator, std::size_t objNumber,
			VkBufferUsageFlags usage, VmaMemoryUsage memoryUsage)
	{
		return { allocator, sizeof(T) * objNumber, usage, memoryUsage };
	}

} // Concerto::Graphics::Wrapper
#endif //CONCERTOGRAPHICS_ALLOCATEDBUFFER_HPP