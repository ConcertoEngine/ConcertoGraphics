//
// Created by arthur on 18/09/2022.
//

#ifndef CONCERTO_GRAPHICS_IMAGEVIEW_HPP
#define CONCERTO_GRAPHICS_IMAGEVIEW_HPP

#include <vulkan/vulkan.h>

#include "Concerto/Graphics/Backend/Vulkan/Defines.hpp"
#include "Concerto/Graphics/Backend/Vulkan/Wrapper/Object.hpp"

namespace Concerto::Graphics::Vk
{
	class Device;
	class Image;

	class CONCERTO_GRAPHICS_VULKAN_BACKEND_API ImageView : public Object<VkImageView>
	{
	 public:
		ImageView(Device& device, Image& image, VkImageAspectFlags aspectFlags);

		~ImageView();

		ImageView(const ImageView&) = delete;

		ImageView(ImageView&&) noexcept = default;

		ImageView& operator=(const ImageView&) = delete;

		ImageView& operator=(ImageView&&) noexcept = default;
	};
}

#endif //CONCERTO_GRAPHICS_IMAGEVIEW_HPP
