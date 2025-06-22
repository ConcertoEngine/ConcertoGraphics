//
// Created by arthur on 09/06/22.
//

#ifndef CONCERTO_GRAPHICS_BUFFER_HPP
#define CONCERTO_GRAPHICS_BUFFER_HPP

#include <cstddef>
#include <functional>

#include <Concerto/Core/Assert.hpp>

#include "Concerto/Graphics/Backend/Vulkan/Wrapper/Object.hpp"
#include "Concerto/Graphics/Backend/Vulkan/Wrapper/Allocator.hpp"

namespace cct::gfx::vk
{
	class CONCERTO_GRAPHICS_VULKAN_BACKEND_API Buffer : public Object<VkBuffer>
	{
	 public:
		Buffer(Allocator& allocator, std::size_t allocSize, VkBufferUsageFlags usage, VmaMemoryUsage memoryUsage, bool allowBufferMapping);

		Buffer(Buffer&&) noexcept;

		Buffer(const Buffer&) = delete;

		Buffer& operator=(Buffer&&) noexcept;

		Buffer& operator=(const Buffer&) = delete;

		~Buffer();

		template<typename T>
		void Copy(T& object, std::size_t padding = 0);

		void Copy(const void* object, std::size_t size, std::size_t padding = 0);

		template<typename DestBuffer, typename SrcObj>
		void Copy(std::vector<SrcObj>& objects, std::function<void(DestBuffer& destBuffer, SrcObj& srcObj)>&& copyFunc, std::size_t padding = 0);

		bool Map(Byte** data);

		template<typename T>
		T* Map();

		void UnMap();

		[[nodiscard]] VmaAllocation GetAllocation() const;
		[[nodiscard]] Allocator& GetAllocator() const;
		[[nodiscard]] std::size_t GetAllocatedSize() const;
		[[nodiscard]] VkBufferUsageFlags GetUsage() const;

	private:
		UInt32 _mapCount = 0;
		std::size_t _allocatedSize;
		Allocator* _allocator;
		VmaAllocation _allocation{ VK_NULL_HANDLE };
		VkBufferUsageFlags _usage;
	};

	/**
	* @brief Utility function to create a AllocatedBuffer object for a specific type.
	*
	* @param allocator The Allocator object used to allocate the memory.
	* @param usage The usage flags for the buffer.
	* @param memoryUsage The memory usage for the allocation.
	* @return A new AllocatedBuffer object with the size of T.
	*/
	template<typename T>
	Buffer MakeBuffer(Allocator& allocator, VkBufferUsageFlags usage, VmaMemoryUsage memoryUsage, bool allowBufferMapping)
	{
		return Buffer { allocator, sizeof(T), usage, memoryUsage, allowBufferMapping };
	}

	/**
	* @brief Utility function to create a AllocatedBuffer object for an array of a specific type.
	*
	* @param allocator The Allocator object used to allocate the memory.
	* @param objNumber The number of objects in the array.
	* @param usage The usage flags for the buffer.
	* @param memoryUsage The memory usage for the allocation.
	* @return A new AllocatedBuffer object with the size of T*objNumber.
	*/
	template<typename T>
	Buffer MakeBuffer(Allocator& allocator,
		std::size_t objNumber,
		VkBufferUsageFlags usage,
		VmaMemoryUsage memoryUsage,
		bool allowBufferMapping)
	{
		return { allocator, sizeof(T) * objNumber, usage, memoryUsage, allowBufferMapping };
	}
} // cct::gfx::vk

#include "Concerto/Graphics/Backend/Vulkan/Wrapper/Buffer.inl"

#endif //CONCERTO_GRAPHICS_BUFFER_HPP
