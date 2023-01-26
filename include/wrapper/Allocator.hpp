//
// Created by arthu on 21/06/2022.
//

#ifndef CONCERTOGRAPHICS_ALLOCATOR_HPP
#define CONCERTOGRAPHICS_ALLOCATOR_HPP

#include "vulkan/vulkan.h"
#include "vk_mem_alloc.h"
#include "wrapper/Object.hpp"

namespace Concerto::Graphics::Wrapper
{
	class Instance;

	class PhysicalDevice;

	class Device;

	/**
	* @class Allocator
	* @brief Represents a VmaAllocator object.
	*
	*  Allocator class is a wrapper for VmaAllocator, it encapsulate the VmaAllocator object,
	*  it also provide some utility functions for creating the VmaAllocator.
	*
	*/
	class Allocator : public Object<VmaAllocator>
	{
	public:
		/**
		* @brief Constructs a new Allocator object.
		*
		* @param physicalDevice The PhysicalDevice object used to create the allocator.
		* @param device The Device object used to create the allocator.
		* @param instance The Instance object used to create the allocator.
		*/
		Allocator(PhysicalDevice& physicalDevice, Device& device, Instance& instance);

		Allocator(const Allocator&) = delete;

		Allocator(Allocator&&) = default;

		Allocator& operator=(const Allocator&) = delete;

		Allocator& operator=(Allocator&&) = default;

		/**
		* @brief Returns the device used to create the allocator
		* @return Reference to the Device object used to create the allocator.
		*/
		[[nodiscard]] Device& GetDevice() const;

	private:
		Device* _device;
	};
}

#endif //CONCERTOGRAPHICS_ALLOCATOR_HPP
