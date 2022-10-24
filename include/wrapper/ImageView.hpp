//
// Created by arthur on 18/09/2022.
//

#ifndef CONCERTOGRAPHICS_IMAGEVIEW_HPP
#define CONCERTOGRAPHICS_IMAGEVIEW_HPP

#include "AllocatedImage.hpp"

namespace Concerto::Graphics::Wrapper
{
	class ImageView
	{
	public:
		ImageView() = default;

		ImageView(AllocatedImage& image, VkImageAspectFlags aspectFlags, VkDevice device);

		~ImageView();

		VkImageView* Get();

	private:
		VkImageView _imageView{ VK_NULL_HANDLE };
		VkDevice _device;
	};
}


#endif //CONCERTOGRAPHICS_IMAGEVIEW_HPP
