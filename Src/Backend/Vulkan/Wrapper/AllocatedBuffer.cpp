//
// Created by arthur on 21/06/2022.
//

#include <stdexcept>
#include <utility>
#include <vk_mem_alloc.h>

#include "Concerto/Graphics/Backend/Vulkan/Wrapper/Buffer.hpp"
#include "Concerto/Graphics/Backend/Vulkan/Wrapper/Device.hpp"

namespace cct::gfx::vk
{

	Buffer::Buffer(Allocator& allocator, std::size_t allocSize, VkBufferUsageFlags usage, VmaMemoryUsage memoryUsage, bool allowBufferMapping) :
		Object(allocator.GetDevice()),
		_allocatedSize(allocSize),
		_allocator(&allocator),
		_usage(usage)
	{
		VkBufferCreateInfo bufferInfo = {};
		bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
		bufferInfo.pNext = nullptr;

		bufferInfo.size = allocSize;
		bufferInfo.usage = usage;

		VmaAllocationCreateInfo vmaAllocInfo = {};
		vmaAllocInfo.usage = memoryUsage;
		vmaAllocInfo.flags = allowBufferMapping ? VMA_ALLOCATION_CREATE_HOST_ACCESS_SEQUENTIAL_WRITE_BIT | VMA_ALLOCATION_CREATE_MAPPED_BIT : 0;
		_lastResult = vmaCreateBuffer(*allocator.Get(), &bufferInfo, &vmaAllocInfo, &_handle, &_allocation, nullptr);
		CCT_ASSERT(_lastResult == VK_SUCCESS, "ConcertoGraphics: vmaCreateBuffer failed VkResult={}", static_cast<int>(_lastResult));
	}

	Buffer::Buffer(Buffer&& other) noexcept :
		Object(other._allocator->GetDevice()),
		_allocatedSize(other._allocatedSize),
		_allocator(other._allocator),
		_usage(other._usage)
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
			CCT_ASSERT_FALSE("ConcertoGraphics: Trying to destroy a buffer that is mapped");
			return;
		}
		_allocator->GetDevice().WaitIdle();
		vmaDestroyBuffer(*_allocator->Get(), _handle, _allocation);
	}

	void Buffer::Copy(const void* object, std::size_t size, std::size_t padding)
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
			CCT_ASSERT_FALSE("ConcertoGraphics: Cannot map buffer");
			return false;
		}
		_mapCount++;
		return true;
	}

	void Buffer::UnMap()
	{
		if (_mapCount == 0)
		{
			CCT_ASSERT_FALSE("ConcertoGraphics: Trying to destroy a buffer that is mapped");
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
		CCT_ASSERT(_allocator, "ConcertoGraphics: allocator is null");
		return *_allocator;
	}

	std::size_t Buffer::GetAllocatedSize() const
	{
		return _allocatedSize;
	}

	VkBufferUsageFlags Buffer::GetUsage() const
	{
		return _usage;
	}
}
