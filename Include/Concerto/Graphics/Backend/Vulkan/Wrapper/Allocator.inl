//
// Created by arthu on 24/08/2025.
//

#ifndef CONCERTO_GRAPHICS_ALLOCATOR_INL
#define CONCERTO_GRAPHICS_ALLOCATOR_INL

#include "Concerto/Graphics/Backend/Vulkan/Wrapper/Allocator.hpp"

namespace cct::gfx::vk
{
	Buffer Allocator::AllocateBuffer(std::size_t allocSize, VkBufferUsageFlags usage, VmaMemoryUsage memoryUsage,
		bool allowBufferMapping)
	{
		return { *this, allocSize, usage, memoryUsage, allowBufferMapping };
	}

	template <typename T>
	Buffer Allocator::AllocateBuffer(VkBufferUsageFlags usage, VmaMemoryUsage memoryUsage, bool allowBufferMapping)
	{
		return { *this, sizeof(T), usage, memoryUsage, allowBufferMapping };
	}

	template <typename T>
	Buffer Allocator::AllocateBuffer(std::size_t objNumber, VkBufferUsageFlags usage, VmaMemoryUsage memoryUsage,
		bool allowBufferMapping)
	{
		return { *this, sizeof(T) * objNumber, usage, memoryUsage, allowBufferMapping };
	}
}

#endif //CONCERTO_GRAPHICS_ALLOCATOR_INL
