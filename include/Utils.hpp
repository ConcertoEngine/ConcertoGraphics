//
// Created by arthur on 14/09/2022.
//

#ifndef CONCERTOGRAPHICS_UTILS_HPP
#define CONCERTOGRAPHICS_UTILS_HPP

#include <vector>
#include <functional>
#include "wrapper/Allocator.hpp"
#include "wrapper/AllocatedBuffer.hpp"

namespace Concerto::Graphics
{
	template<typename T>
	void MapAndCopy(Wrapper::Allocator& allocator, Wrapper::AllocatedBuffer& buffer, T& object)
	{
		void* data;
		vmaMapMemory(allocator._allocator, buffer._allocation, &data);
		std::memcpy(data, &object, sizeof(T));
		vmaUnmapMemory(allocator._allocator, buffer._allocation);
	}

	template<typename T>
	void MapAndCopy(Wrapper::Allocator& allocator, Wrapper::AllocatedBuffer& buffer, T* object, std::size_t size)
	{
		void* data;
		vmaMapMemory(allocator._allocator, buffer._allocation, &data);
		std::memcpy(data, object, size);
		vmaUnmapMemory(allocator._allocator, buffer._allocation);
	}

	template<typename DestBuffer, typename SrcObj>
	void MapAndCopy(Wrapper::Allocator& allocator, Wrapper::AllocatedBuffer& buffer, std::vector<SrcObj>& objects,
			std::function<void(DestBuffer& destBuffer, SrcObj& srcObj)> && copyFunc)
	{
		void* data;
		vmaMapMemory(allocator._allocator, buffer._allocation, &data);
		auto* destBuffer = reinterpret_cast<DestBuffer*>(data);
		for(std::size_t i = 0; i < objects.size(); i++)
		{
			copyFunc(destBuffer[i], objects[i]);
		}
		vmaUnmapMemory(allocator._allocator, buffer._allocation);
	}

	template<typename T>
	void MapAndCopy(Wrapper::Allocator& allocator, Wrapper::AllocatedBuffer& buffer, T& object, std::size_t padding)
	{
		char* data;
		vmaMapMemory(allocator._allocator, buffer._allocation, (void**)&data);
		data += padding;
		std::memcpy(data, &object, sizeof(T));
		vmaUnmapMemory(allocator._allocator, buffer._allocation);
	}

	inline std::size_t PadUniformBuffer(std::size_t size, std::size_t minUniformBufferOffsetAlignment)
	{
		if (size > 0)
			return (size + minUniformBufferOffsetAlignment - 1) & ~(minUniformBufferOffsetAlignment - 1);
		return size;
	}
}

#endif //CONCERTOGRAPHICS_UTILS_HPP
