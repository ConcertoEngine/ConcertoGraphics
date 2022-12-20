//
// Created by arthur on 21/06/2022.
//

#include <stdexcept>
#include <utility>
#include "wrapper/AllocatedBuffer.hpp"
#include "wrapper/Device.hpp"

namespace Concerto::Graphics::Wrapper
{

	AllocatedBuffer::AllocatedBuffer(Allocator& allocator,
			std::size_t allocSize, VkBufferUsageFlags usage, VmaMemoryUsage memoryUsage) : _allocator(&allocator)
	{
		VkBufferCreateInfo bufferInfo = {};
		bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
		bufferInfo.pNext = nullptr;

		bufferInfo.size = allocSize;
		bufferInfo.usage = usage;

		VmaAllocationCreateInfo vmaAllocInfo = {};
		vmaAllocInfo.usage = memoryUsage;

		if (vmaCreateBuffer(*allocator.Get(), &bufferInfo, &vmaAllocInfo, &_buffer, &_allocation, nullptr) !=
			VK_SUCCESS)
		{
			throw std::runtime_error("Failed to allocate command buffer");
		}
	}

	AllocatedBuffer::AllocatedBuffer(AllocatedBuffer&& other) noexcept
	{
		_allocator = std::exchange(other._allocator, nullptr);
		_buffer = std::exchange(other._buffer, VK_NULL_HANDLE);
		_allocation = std::exchange(other._allocation, nullptr);
	}

	AllocatedBuffer& AllocatedBuffer::operator=(AllocatedBuffer&& other) noexcept
	{
		return *this;
	}

	AllocatedBuffer::~AllocatedBuffer()
	{
		if (_buffer == VK_NULL_HANDLE)
			return;
		_allocator->GetDevice().WaitIdle();
		vmaDestroyBuffer(*_allocator->Get(), _buffer, _allocation);
	}
}