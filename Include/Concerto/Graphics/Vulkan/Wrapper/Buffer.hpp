//
// Created by arthur on 09/06/22.
//

#ifndef CONCERTO_GRAPHICS_BUFFER_HPP
#define CONCERTO_GRAPHICS_BUFFER_HPP

#include <cstddef>
#include <cstring>

#include <Concerto/Core/Assert.hpp>

#include "Concerto/Graphics/Vulkan/Wrapper/Object.hpp"
#include "Concerto/Graphics/Vulkan/Wrapper/Allocator.hpp"

namespace Concerto::Graphics
{
	/**
	 * @class Buffer
	 * @brief Represents a VkBuffer object with its allocation.
	 *
	 * Buffer class is a Wrapper for VkBuffer, it encapsulate the VkBuffer object and its corresponding allocation
	 */
	class CONCERTO_GRAPHICS_API Buffer : public Object<VkBuffer>
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

		~Buffer() = default;

		template<typename T>
		void Copy(T& object, std::size_t padding = 0)
		{
			Byte* data = nullptr;
			if(Map(&data) == false)
			{
				CONCERTO_ASSERT_FALSE;
				return;
			}
			data += padding;
			std::memcpy(data, &object, sizeof(T));
			UnMap();
		}

		void Copy(void* object, std::size_t size, std::size_t padding = 0);

		template<typename DestBuffer, typename SrcObj>
		void Copy(std::vector<SrcObj>& objects, std::function<void(DestBuffer& destBuffer, SrcObj& srcObj)>&& copyFunc, std::size_t padding = 0)
		{
			Byte* data = nullptr;
			if (Map(&data) == false)
			{
				CONCERTO_ASSERT_FALSE;
				return;
			}
			data += padding;
			auto* destBuffer = static_cast<DestBuffer*>(data);
			for (std::size_t i = 0; i < objects.size(); i++)
			{
				copyFunc(destBuffer[i], objects[i]);
			}
			UnMap();
		}

		bool Map(Byte** data);

		template<typename T>
		T* Map()
		{
			Byte* data = nullptr;
			if (Map(&data) == false)
				CONCERTO_ASSERT_FALSE;
			return reinterpret_cast<T*>(data);
		}

		void UnMap();

		/**
		 * @return The allocation object.
		 */
		[[nodiscard]] VmaAllocation GetAllocation() const;

		/**
		 * @return The allocator which allocated the memory.
		 */
		[[nodiscard]] Allocator& GetAllocator() const;

		/**
		 * @return The allocated size
		 */
		[[nodiscard]] std::size_t GetAllocatedSize() const;

	private:
		UInt32 _mapCount = 0;
		std::size_t _allocatedSize;
		Allocator& _allocator;
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
#endif //CONCERTO_GRAPHICS_BUFFER_HPP