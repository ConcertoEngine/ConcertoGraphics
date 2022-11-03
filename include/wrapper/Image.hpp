//
// Created by arthur on 09/06/22.
//

#ifndef CONCERTOGRAPHICS_IMAGE_HPP
#define CONCERTOGRAPHICS_IMAGE_HPP

#include <string>
#include <vulkan/vulkan.h>
#include <vk_mem_alloc.h>
#include "wrapper/Object.hpp"
#include "wrapper/Allocator.hpp"
#include "VulkanInitializer.hpp"

namespace Concerto::Graphics
{
	class UploadContext;
}
namespace Concerto::Graphics::Wrapper
{
	class CommandBuffer;

	class Queue;

	class Device;

	class Image : public Object<VkImage>
	{
	public:
		explicit Image(Device& device, VkImage image, VkFormat imageFormat);

		Image(Device& device, VkExtent2D extent, VkFormat depthFormat, Allocator& allocator);

		Image(Device& device, const std::string& file, Allocator& allocator, CommandBuffer& commandBuffer,
				UploadContext& uploadContext, Queue& queue);

		Image(Image&&) = default;

		Image(const Image&) = delete;

		Image& operator=(Image&&) = default;

		Image& operator=(const Image&) = delete;

		~Image();

		VkFormat GetFormat() const;

	private:
		bool _isAllocated;
		VkFormat _imageFormat{};
		VmaAllocation _allocation{};
		VmaAllocator _allocator;
	};
}

#endif //CONCERTOGRAPHICS_IMAGE_HPP