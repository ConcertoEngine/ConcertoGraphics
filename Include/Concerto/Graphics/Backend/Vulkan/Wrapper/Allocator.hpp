//
// Created by arthu on 21/06/2022.
//

#ifndef CONCERTO_GRAPHICS_ALLOCATOR_HPP
#define CONCERTO_GRAPHICS_ALLOCATOR_HPP

#include <vk_mem_alloc.h>

#include "Concerto/Graphics/Backend/Vulkan/Defines.hpp"
#include "Concerto/Graphics/Backend/Vulkan/Wrapper/Object.hpp"

namespace cct::gfx::vk
{
	class Instance;
	class PhysicalDevice;
	class Device;

	class CONCERTO_GRAPHICS_VULKAN_BACKEND_API Allocator : public Object<VmaAllocator>
	{
	public:
		Allocator(PhysicalDevice& physicalDevice, Device& device, Instance& instance);

		Allocator(const Allocator&) = delete;

		Allocator(Allocator&&) = default;

		Allocator& operator=(const Allocator&) = delete;

		Allocator& operator=(Allocator&&) = default;

		~Allocator() override;
	};
}

#endif //CONCERTO_GRAPHICS_ALLOCATOR_HPP
