//
// Created by arthur on 21/06/2022.
//

#include "wrapper/AllocatedBuffer.hpp"
#include <stdexcept>

namespace Concerto::Graphics::Wrapper
{

	AllocatedBuffer::AllocatedBuffer(Allocator& allocator,
			std::size_t allocSize, VkBufferUsageFlags usage, VmaMemoryUsage memoryUsage) : _allocator(allocator)
	{
		VkBufferCreateInfo bufferInfo = {};
		bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
		bufferInfo.pNext = nullptr;

		bufferInfo.size = allocSize;
		bufferInfo.usage = usage;

		VmaAllocationCreateInfo vmaAllocInfo = {};
		vmaAllocInfo.usage = memoryUsage;

		if (vmaCreateBuffer(allocator._allocator, &bufferInfo, &vmaAllocInfo, &_buffer, &_allocation, nullptr) !=
			VK_SUCCESS)
		{
			throw std::runtime_error("Failed to allocate command buffer");
		}
	}

	AllocatedBuffer::~AllocatedBuffer()
	{
		vmaDestroyBuffer(_allocator._allocator, _buffer, _allocation);
	}
}