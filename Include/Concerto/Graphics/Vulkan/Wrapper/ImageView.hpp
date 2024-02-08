//
// Created by arthur on 18/09/2022.
//

#ifndef CONCERTO_GRAPHICS_IMAGEVIEW_HPP
#define CONCERTO_GRAPHICS_IMAGEVIEW_HPP

#include <vulkan/vulkan.h>
#include "Concerto/Graphics/Defines.hpp"

#include "Concerto/Graphics/Vulkan/Wrapper/Object.hpp"

namespace Concerto::Graphics
{
	class Device;
	class Image;

	class CONCERTO_GRAPHICS_API ImageView : public Object<VkImageView>
	{
	 public:
		ImageView(Device& device, Image& image, VkImageAspectFlags aspectFlags);

		ImageView(const ImageView&) = delete;

		ImageView(ImageView&&) noexcept = default;

		ImageView& operator=(const ImageView&) = delete;

		ImageView& operator=(ImageView&&) noexcept = default;
	};
}

#endif //CONCERTO_GRAPHICS_IMAGEVIEW_HPP
