//
// Created by arthur on 09/06/22.
//

#ifndef CONCERTOGRAPHICS_BUFFER_HPP
#define CONCERTOGRAPHICS_BUFFER_HPP

#include <cstddef>
#include <vulkan/vulkan.h>
#include <vk_mem_alloc.h>
#include <Concerto/Core/Types.hpp>
#include "Concerto/Graphics/Vulkan/Wrapper/Allocator.hpp"

namespace Concerto::Graphics
{
	/**
	 * @class Buffer
	 * @brief Represents a VkBuffer object with its allocation.
	 *
	 * Buffer class is a Wrapper for VkBuffer, it encapsulate the VkBuffer object and its corresponding allocation
	 */
	class CONCERTO_PUBLIC_API Buffer
	{
	 public:
		/**
		* @brief Constructs a new AllocatedBuffer object.
		*
		* @param allocator The Allocator object used to allocate the memory.
		* @param allocSize The size of the allocation.
		* @param usage The usage flags for the buffer.
		* @param memoryUsage The memory usage for the allocation.
		*/
		Buffer(Allocator& allocator, std::size_t allocSize, VkBufferUsageFlags usage, VmaMemoryUsage memoryUsage);

		Buffer(Buffer&&) noexcept;

		Buffer(const Buffer&) = delete;

		Buffer& operator=(Buffer&&) noexcept;

		Buffer& operator=(const Buffer&) = delete;

		~Buffer();

		Allocator* _allocator;
		VkBuffer _buffer{ VK_NULL_HANDLE };
		VmaAllocation _allocation{ VK_NULL_HANDLE };
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
	Buffer MakeBuffer(Allocator& allocator, VkBufferUsageFlags usage, VmaMemoryUsage memoryUsage)
	{
		return { allocator, sizeof(T), usage, memoryUsage };
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
		VmaMemoryUsage memoryUsage)
	{
		return { allocator, sizeof(T) * objNumber, usage, memoryUsage };
	}

} // Concerto::Graphics::Wrapper
#endif //CONCERTOGRAPHICS_BUFFER_HPP