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

		~Image() override;

		Image(Image&&) noexcept;
		Image(const Image&) = delete;

		Image& operator=(Image&&) noexcept;
		Image& operator=(const Image&) = delete;

		[[nodiscard]] VkFormat GetFormat() const;
		VkExtent2D GetExtent() const;
	private:
		bool m_isAllocated;
		VkFormat m_imageFormat = {};
		VmaAllocation m_allocation = {};
		VkExtent2D m_extent;
	};
}

#endif //CONCERTO_GRAPHICS_IMAGE_HPP