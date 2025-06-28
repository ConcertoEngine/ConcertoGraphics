//
// Created by arthur on 09/06/22.
//

#ifndef CONCERTO_GRAPHICS_IMAGE_HPP
#define CONCERTO_GRAPHICS_IMAGE_HPP

#include <string>

#include <vk_mem_alloc.h>

#include "Concerto/Graphics/Backend/Vulkan/Defines.hpp"
#include "Concerto/Graphics/Backend/Vulkan/Wrapper/Object.hpp"

namespace cct::gfx::vk
{
	class UploadContext;
	class CommandBuffer;
	class Queue;
	class Device;

	class CONCERTO_GRAPHICS_VULKAN_BACKEND_API Image : public Object<VkImage>
	{
	public:
		Image(Device& device, VkExtent2D extent, VkImage image, VkFormat imageFormat);
		Image(Device& device, VkExtent2D extent, VkFormat format, VkImageUsageFlags usageFlags);

		~Image();

		Image(Image&&) noexcept;

		Image(const Image&) = delete;

		Image& operator=(Image&&) noexcept;

		Image& operator=(const Image&) = delete;

		/**
		* @brief Gets the format of the image.
		*
		* @return The format of the image.
		*/
		[[nodiscard]] VkFormat GetFormat() const;

		VkExtent2D GetExtent() const;

	private:
		bool _isAllocated;
		VkFormat _imageFormat = {};
		VmaAllocation _allocation = {};
		VkExtent2D _extent;
	};
}

#endif //CONCERTO_GRAPHICS_IMAGE_HPP