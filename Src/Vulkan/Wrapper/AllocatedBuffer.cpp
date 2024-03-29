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
		Object(allocator.GetDevice()),
		_allocatedSize(allocSize),
		_allocator(&allocator)
	{
		VkBufferCreateInfo bufferInfo = {};
		bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
		bufferInfo.pNext = nullptr;

		bufferInfo.size = allocSize;
		bufferInfo.usage = usage;

		VmaAllocationCreateInfo vmaAllocInfo = {};
		vmaAllocInfo.usage = memoryUsage;
		_lastResult = vmaCreateBuffer(*allocator.Get(), &bufferInfo, &vmaAllocInfo, &_handle, &_allocation, nullptr);
		CONCERTO_ASSERT(_lastResult == VK_SUCCESS, "ConcertoGraphics: vmaCreateBuffer failed VkResult={}", static_cast<int>(_lastResult));
	}

	Buffer::Buffer(Buffer&& other) noexcept :
		Object(other._allocator->GetDevice()),
		_allocatedSize(other._allocatedSize),
		_allocator(other._allocator)
	{
		_allocator = std::exchange(other._allocator, nullptr);
		_handle = std::exchange(other._handle, VK_NULL_HANDLE);
		_allocation = std::exchange(other._allocation, nullptr);
	}

	Buffer& Buffer::operator=(Buffer&& other) noexcept
	{
		_allocator = std::exchange(other._allocator, nullptr);
		_handle = std::exchange(other._handle, VK_NULL_HANDLE);
		_allocation = std::exchange(other._allocation, nullptr);
		return *this;
	}

	Buffer::~Buffer()
	{
		if (IsNull())
			return;
		if (_mapCount != 0)
		{
			CONCERTO_ASSERT_FALSE("ConcertoGraphics: Trying to destroy a buffer that is mapped");
			return;
		}
		_allocator->GetDevice().WaitIdle();
		vmaDestroyBuffer(*_allocator->Get(), _handle, _allocation);
	}

	void Buffer::Copy(void* object, std::size_t size, std::size_t padding)
	{
		Byte* data = nullptr;
		if (Map(&data) == false)
			return;
		data += padding;
		std::memcpy(data, object, size);
		UnMap();
	}

	bool Buffer::Map(Byte** data)
	{
		const auto res = vmaMapMemory(*_allocator->Get(), _allocation, reinterpret_cast<void**>(data));
		if (res != VK_SUCCESS)
		{
			CONCERTO_ASSERT_FALSE("ConcertoGraphics: Cannot map buffer");
			return false;
		}
		_mapCount++;
		return true;
	}
	
	void Buffer::UnMap()
	{
		if (_mapCount == 0)
		{
			CONCERTO_ASSERT_FALSE("ConcertoGraphics: Trying to destroy a buffer that is mapped");
			return;
		}
		_mapCount--;
		vmaUnmapMemory(*_allocator->Get(), _allocation);
	}

	VmaAllocation Buffer::GetAllocation() const
	{
		return _allocation;
	}

	Allocator& Buffer::GetAllocator() const
	{
		CONCERTO_ASSERT(_allocator, "ConcertoGraphics: allocator is null");
		return *_allocator;
	}

	std::size_t Buffer::GetAllocatedSize() const
	{
		return _allocatedSize;
	}
}
