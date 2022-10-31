//
// Created by arthur on 09/06/22.
//

#ifndef CONCERTOGRAPHICS_IMAGE_HPP
#define CONCERTOGRAPHICS_IMAGE_HPP

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

	struct Image
	{
		explicit Image(VkImage image, VkFormat imageFormat);

		Image(VkExtent2D extent, VkFormat depthFormat, Allocator& allocator);

		Image(const std::string& file, Allocator& allocator, CommandBuffer& commandBuffer,
				UploadContext& uploadContext, Queue& queue);

		Image(Image&&) = default;

		Image(const Image&) = delete;

		Image& operator=(Image&&) = default;

		Image& operator=(const Image&) = delete;

		~Image();

		VkImage* Get();

		VkFormat GetFormat() const;

	private:
		bool _isAllocated;
		VkFormat _imageFormat{};
		VkImage _image{};
		VmaAllocation _allocation{};
		VmaAllocator _allocator;
	};
}

#endif //CONCERTOGRAPHICS_IMAGE_HPP