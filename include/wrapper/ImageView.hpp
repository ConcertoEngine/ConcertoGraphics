//
// Created by arthur on 18/09/2022.
//

#ifndef CONCERTOGRAPHICS_IMAGEVIEW_HPP
#define CONCERTOGRAPHICS_IMAGEVIEW_HPP

#include "Image.hpp"

namespace Concerto::Graphics::Wrapper
{
	class ImageView
	{
	public:
		ImageView() = default;

		ImageView(Image& image, VkImageAspectFlags aspectFlags, VkDevice device);

		ImageView(const ImageView&) = delete;

		ImageView(ImageView&&) noexcept;

		ImageView& operator=(const ImageView&) = delete;

		ImageView& operator=(ImageView&&) noexcept;

		~ImageView();

		VkImageView* Get();

	private:
		VkImageView _imageView{ VK_NULL_HANDLE };
		VkDevice _device{ VK_NULL_HANDLE };
	};
}


#endif //CONCERTOGRAPHICS_IMAGEVIEW_HPP
