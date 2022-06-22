//
// Created by arthur on 09/06/22.
//

#ifndef CONCERTOGRAPHICS_ALLOCATEDIMAGE_HPP
#define CONCERTOGRAPHICS_ALLOCATEDIMAGE_HPP

#include <vulkan/vulkan.h>
#include <vk_mem_alloc.h>

namespace Concerto::Graphics::Wrapper
{
	struct AllocatedImage
	{
		AllocatedImage() = default;

		AllocatedImage(AllocatedImage&&) = default;

		AllocatedImage(const AllocatedImage&) = delete;

		AllocatedImage& operator=(AllocatedImage&&) = default;

		AllocatedImage& operator=(const AllocatedImage&) = delete;

		~AllocatedImage() = default;
		VkImage _image;
		VmaAllocation _allocation;
	};

}

#endif //CONCERTOGRAPHICS_ALLOCATEDIMAGE_HPP