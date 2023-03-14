//
// Created by arthur on 18/09/2022.
//

#ifndef CONCERTOGRAPHICS_TEXTURE_HPP
#define CONCERTOGRAPHICS_TEXTURE_HPP

#include <memory>
#include <vulkan/vulkan.h>
#include "Vulkan/Wrapper/Image.hpp"
#include "Vulkan/Wrapper/ImageView.hpp"
#include "Vulkan/Wrapper/CommandBuffer.hpp"
#include "Vulkan/Wrapper/Device.hpp"

namespace Concerto::Graphics
{
	struct Texture
	{
		Texture(Wrapper::Device& device, const std::string& file, Wrapper::Allocator& allocator,
				Wrapper::CommandBuffer& commandBuffer, UploadContext& uploadContext, Wrapper::Queue& queue,
				VkImageAspectFlags aspectFlags) :
				_image(device, file, allocator, commandBuffer, uploadContext, queue),
				_imageView(device, _image, aspectFlags)
		{

		}

		Wrapper::Image _image;
		Wrapper::ImageView _imageView;
	};
	using TexturePtr = std::shared_ptr<Texture>;
}
#endif //CONCERTOGRAPHICS_TEXTURE_HPP
