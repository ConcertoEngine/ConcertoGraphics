//
// Created by arthur on 21/06/2022.
//

#include <stdexcept>
#include <utility>

#include "Concerto/Graphics/Vulkan/Wrapper/Buffer.hpp"
#include "Concerto/Graphics/Vulkan/Wrapper/Device.hpp"

namespace Concerto::Graphics
{

	Buffer::Buffer(Allocator& allocator, std::size_t allocSize, VkBufferUsageFlags usage, VmaMemoryUsage memoryUsage) : 
		_allocator(allocator)
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
			throw std::runtime_error("Failed to allocate buffer");
		}
	}

	Buffer::Buffer(Buffer&& other) noexcept :
		_allocator(other._allocator)
	{
		//_allocator = std::exchange(other._allocator, nullptr);
		_buffer = std::exchange(other._buffer, VK_NULL_HANDLE);
		_allocation = std::exchange(other._allocation, nullptr);
	}

	Buffer& Buffer::operator=(Buffer&& other) noexcept
	{
		return *this;
	}

	Buffer::~Buffer()
	{
		if (_buffer == VK_NULL_HANDLE)
			return;
		_allocator.GetDevice().WaitIdle();
		vmaDestroyBuffer(*_allocator.Get(), _buffer, _allocation);
	}

	bool Buffer::Map(void** data)
	{
		return vmaMapMemory(*_allocator.Get(), _allocation, data) == VK_SUCCESS;
	}
	
	void Buffer::UnMap()
	{
		vmaUnmapMemory(*_allocator.Get(), _allocation);
	}
}