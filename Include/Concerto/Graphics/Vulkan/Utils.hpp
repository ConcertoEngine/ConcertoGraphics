//
// Created by arthur on 14/09/2022.
//

#ifndef CONCERTOGRAPHICS_UTILS_HPP
#define CONCERTOGRAPHICS_UTILS_HPP

#include <vector>
#include <functional>

#include <glm/vec4.hpp>
#include <glm/mat4x4.hpp>

#include "Concerto/Graphics/Vulkan/Wrapper/Allocator.hpp"
#include "Concerto/Graphics/Vulkan/Wrapper/Buffer.hpp"
#include "Concerto/Graphics/Vulkan/Wrapper/Device.hpp"

namespace Concerto::Graphics
{
	struct MeshPushConstants
	{
		glm::vec4 data;
		glm::mat4 render_matrix;
	};

	template<typename T>
	void MapAndCopy(Allocator& allocator, Buffer& buffer, T& object)
	{
		void* data;
		vmaMapMemory(*allocator.Get(), buffer._allocation, &data);
		std::memcpy(data, &object, sizeof(T));
		vmaUnmapMemory(*allocator.Get(), buffer._allocation);
	}

	template<typename T>
	void MapAndCopy(Allocator& allocator, Buffer& buffer, T* object, std::size_t size)
	{
		void* data;
		vmaMapMemory(*allocator.Get(), buffer._allocation, &data);
		std::memcpy(data, object, size);
		vmaUnmapMemory(*allocator.Get(), buffer._allocation);
	}

	template<typename DestBuffer, typename SrcObj>
	void MapAndCopy(Allocator& allocator, Buffer& buffer, std::vector<SrcObj>& objects,
		std::function<void(DestBuffer& destBuffer, SrcObj& srcObj)>&& copyFunc)
	{
		void* data;
		vmaMapMemory(*allocator.Get(), buffer._allocation, &data);
		auto* destBuffer = reinterpret_cast<DestBuffer*>(data);
		for (std::size_t i = 0; i < objects.size(); i++)
		{
			copyFunc(destBuffer[i], objects[i]);
		}
		vmaUnmapMemory(*allocator.Get(), buffer._allocation);
	}

	template<typename T>
	void MapAndCopy(Allocator& allocator, Buffer& buffer, T& object, std::size_t padding)
	{
		char* data;
		vmaMapMemory(*allocator.Get(), buffer._allocation, (void**)&data);
		data += padding;
		std::memcpy(data, &object, sizeof(T));
		vmaUnmapMemory(*allocator.Get(), buffer._allocation);
	}

	inline std::size_t PadUniformBuffer(std::size_t size, std::size_t minUniformBufferOffsetAlignment)
	{
		if (size > 0)
			return (size + minUniformBufferOffsetAlignment - 1) & ~(minUniformBufferOffsetAlignment - 1);
		return size;
	}

	template<typename T>
	inline T* MapBuffer(Buffer& buffer)
	{
		void* data;
		vmaMapMemory(*buffer._allocator->Get(), buffer._allocation, &data);
		return reinterpret_cast<T*>(data);
	}

	inline void UnMapBuffer(Buffer& buffer)
	{
		vmaUnmapMemory(*buffer._allocator->Get(), buffer._allocation);
	}
}

#endif //CONCERTOGRAPHICS_UTILS_HPP
