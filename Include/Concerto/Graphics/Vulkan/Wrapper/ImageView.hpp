//
// Created by arthur on 18/09/2022.
//

#ifndef CONCERTOGRAPHICS_IMAGEVIEW_HPP
#define CONCERTOGRAPHICS_IMAGEVIEW_HPP

#include <vulkan/vulkan.h>
#include <Concerto/Core/Types.hpp>

#include "Concerto/Graphics/Vulkan/Wrapper/Object.hpp"

namespace Concerto::Graphics
{
	class Device;
	class Image;

	class CONCERTO_PUBLIC_API ImageView : public Object<VkImageView>
	{
	 public:
		ImageView(Device& device, Image& image, VkImageAspectFlags aspectFlags);

		ImageView(const ImageView&) = delete;

		ImageView(ImageView&&) noexcept = default;

		ImageView& operator=(const ImageView&) = delete;

		ImageView& operator=(ImageView&&) noexcept = default;
	};
}

#endif //CONCERTOGRAPHICS_IMAGEVIEW_HPP
