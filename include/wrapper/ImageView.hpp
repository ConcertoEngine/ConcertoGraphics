//
// Created by arthur on 18/09/2022.
//

#ifndef CONCERTOGRAPHICS_IMAGEVIEW_HPP
#define CONCERTOGRAPHICS_IMAGEVIEW_HPP

#include "vulkan/vulkan.h"
#include "wrapper/Object.hpp"
namespace Concerto::Graphics::Wrapper
{
	class Device;
	class Image;
	class ImageView : public Object<VkImageView>
	{
	public:
		ImageView(Device& device, Image& image, VkImageAspectFlags aspectFlags);

		ImageView(const ImageView&) = delete;

		ImageView(ImageView&&) noexcept = default;

		ImageView& operator=(const ImageView&) = delete;

		ImageView& operator=(ImageView&&) noexcept = default;

		~ImageView();
	};
}


#endif //CONCERTOGRAPHICS_IMAGEVIEW_HPP
