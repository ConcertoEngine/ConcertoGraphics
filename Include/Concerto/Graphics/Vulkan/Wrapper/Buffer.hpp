//
// Created by arthur on 09/06/22.
//

#ifndef CONCERTOGRAPHICS_BUFFER_HPP
#define CONCERTOGRAPHICS_BUFFER_HPP

#include <cstddef>
#include <cstring>
#include "Concerto/Graphics/Vulkan/Wrapper/Allocator.hpp"

namespace Concerto::Graphics
{
	/**
	 * @class Buffer
	 * @brief Represents a VkBuffer object with its allocation.
	 *
	 * Buffer class is a Wrapper for VkBuffer, it encapsulate the VkBuffer object and its corresponding allocation
	 */
	class CONCERTO_GRAPHICS_API Buffer
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

		template<typename T>
		inline void Copy(T& object)
		{
			void* data = nullptr;
			if(Map(&data) == false)
				CONCERTO_ASSERT_FALSE;
			std::memcpy(data, &object, sizeof(T));
			UnMap();
		}

		template<typename T>
		inline void Copy(T* object, std::size_t size)
		{
			void* data = nullptr;
			CONCERTO_ASSERT(Map(&data));
			std::memcpy(data, object, size);
			UnMap();
		}

		template<typename DestBuffer, typename SrcObj>
		inline void Copy(std::vector<SrcObj>& objects, std::function<void(DestBuffer& destBuffer, SrcObj& srcObj)>&& copyFunc)
		{
			void* data = nullptr;
			CONCERTO_ASSERT(Map(&data));
			auto* destBuffer = reinterpret_cast<DestBuffer*>(data);
			for (std::size_t i = 0; i < objects.size(); i++)
			{
				copyFunc(destBuffer[i], objects[i]);
			}
			UnMap();
		}

		template<typename T>
		inline void Copy(T& object, std::size_t padding)
		{
			void* data;
			CONCERTO_ASSERT(Map(&data));
			data += padding;
			std::memcpy(data, &object, sizeof(T));
			UnMap();
		}


		bool Map(void** data);

		template<typename T>
		inline T* Map()
		{
			void* data = nullptr;
			if (Map(&data) == false)
				CONCERTO_ASSERT_FALSE;
			return reinterpret_cast<T*>(data);
		}

		void UnMap();

		

		Allocator& _allocator;
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