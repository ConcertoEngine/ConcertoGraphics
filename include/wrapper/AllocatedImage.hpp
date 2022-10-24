//
// Created by arthur on 09/06/22.
//

#ifndef CONCERTOGRAPHICS_ALLOCATEDIMAGE_HPP
#define CONCERTOGRAPHICS_ALLOCATEDIMAGE_HPP

#include <vulkan/vulkan.h>
#include <vk_mem_alloc.h>
#include "VulkanInitializer.hpp"
#include "Allocator.hpp"
#include <string>

namespace Concerto::Graphics
{
	class UploadContext;
}
namespace Concerto::Graphics::Wrapper
{
	class CommandBuffer;

	class Queue;

	struct AllocatedImage
	{
		AllocatedImage(VkExtent2D extent, VkFormat depthFormat, Allocator& allocator);

		AllocatedImage(const std::string& file, VkExtent2D extent, Allocator& allocator, CommandBuffer& commandBuffer,
				UploadContext& uploadContext, Queue& queue);

		AllocatedImage() = default;

		AllocatedImage(AllocatedImage&&) = default;

		AllocatedImage(const AllocatedImage&) = delete;

		AllocatedImage& operator=(AllocatedImage&&) = default;

		AllocatedImage& operator=(const AllocatedImage&) = delete;

		~AllocatedImage() = default;

		VkFormat imageFormat;
		VkImage _image{};
		VmaAllocation _allocation{};
	};
}

#endif //CONCERTOGRAPHICS_ALLOCATEDIMAGE_HPP