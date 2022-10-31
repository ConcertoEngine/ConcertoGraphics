//
// Created by arthur on 18/09/2022.
//

#ifndef CONCERTOGRAPHICS_TEXTURE_HPP
#define CONCERTOGRAPHICS_TEXTURE_HPP

#include <memory>
#include "vulkan/vulkan.h"
#include "wrapper/Image.hpp"
#include "wrapper/ImageView.hpp"
#include "wrapper/CommandBuffer.hpp"

namespace Concerto::Graphics
{
	struct Texture
	{
		Texture(const std::string& file, Wrapper::Allocator& allocator,
				Wrapper::CommandBuffer& commandBuffer, UploadContext& uploadContext, Wrapper::Queue& queue,
				VkImageAspectFlags aspectFlags, VkDevice device) :
				_image(file, allocator, commandBuffer, uploadContext, queue),
				_imageView(_image, aspectFlags, device)
		{

		}

		Wrapper::Image _image;
		Wrapper::ImageView _imageView;
	};
}
#endif //CONCERTOGRAPHICS_TEXTURE_HPP
