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
		Object(allocator.GetDevice(), [&](Device&, VkBuffer buffer)
		{
			if (_mapCount != 0)
			{
				CONCERTO_ASSERT_FALSE; // trying to destroy a buffer that is mapped
				Logger::Error("Trying to destroy a buffer that is mapped");
			}
			_allocator.GetDevice().WaitIdle();
			vmaDestroyBuffer(*_allocator.Get(), buffer, _allocation);
		}),
		_allocatedSize(allocSize),
		_allocator(allocator)
	{
		VkBufferCreateInfo bufferInfo = {};
		bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
		bufferInfo.pNext = nullptr;

		bufferInfo.size = allocSize;
		bufferInfo.usage = usage;

		VmaAllocationCreateInfo vmaAllocInfo = {};
		vmaAllocInfo.usage = memoryUsage;

		if (vmaCreateBuffer(*allocator.Get(), &bufferInfo, &vmaAllocInfo, &_handle, &_allocation, nullptr) !=
			VK_SUCCESS)
		{
			CONCERTO_ASSERT_FALSE; // Failed to allocate buffer
			throw std::runtime_error("Failed to allocate buffer");
		}
	}

	Buffer::Buffer(Buffer&& other) noexcept :
		Object(other._allocator.GetDevice()),
		_allocatedSize(other._allocatedSize),
		_allocator(other._allocator)
	{
		//_allocator = std::exchange(other._allocator, nullptr);
		_handle = std::exchange(other._handle, VK_NULL_HANDLE);
		_allocation = std::exchange(other._allocation, nullptr);
	}

	Buffer& Buffer::operator=(Buffer&& other) noexcept
	{
		return *this;
	}

	void Buffer::Copy(void* object, std::size_t size, std::size_t padding)
	{
		Byte* data = nullptr;
		if (Map(&data) == false)
		{
			CONCERTO_ASSERT_FALSE;
			return;
		}
		data += padding;
		std::memcpy(data, object, size);
		UnMap();
	}

	bool Buffer::Map(Byte** data)
	{
		const auto res = vmaMapMemory(*_allocator.Get(), _allocation, reinterpret_cast<void**>(data));
		if (res != VK_SUCCESS)
		{
			CONCERTO_ASSERT_FALSE; //Cannot map memory
			return false;
		}
		_mapCount++;
		return true;
	}
	
	void Buffer::UnMap()
	{
		if (_mapCount == 0)
			CONCERTO_ASSERT_FALSE; // trying to unmap a buffer that is not mapped
		_mapCount--;
		vmaUnmapMemory(*_allocator.Get(), _allocation);
	}

	VmaAllocation Buffer::GetAllocation() const
	{
		return _allocation;
	}

	Allocator& Buffer::GetAllocator() const
	{
		return _allocator;
	}

	std::size_t Buffer::GetAllocatedSize() const
	{
		return _allocatedSize;
	}
}
