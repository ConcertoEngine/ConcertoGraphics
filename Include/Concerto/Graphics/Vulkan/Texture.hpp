//
// Created by arthur on 18/09/2022.
//

#ifndef CONCERTOGRAPHICS_TEXTURE_HPP
#define CONCERTOGRAPHICS_TEXTURE_HPP

#include <memory>

#include <vulkan/vulkan.h>

#include "Concerto/Graphics/Vulkan/Wrapper/Image.hpp"
#include "Concerto/Graphics/Vulkan/Wrapper/ImageView.hpp"
#include "Concerto/Graphics/Vulkan/Wrapper/CommandBuffer.hpp"
#include "Concerto/Graphics/Vulkan/Wrapper/Device.hpp"

namespace Concerto::Graphics
{
	struct Texture
	{
		Texture(Device& device, const std::string& file, Allocator& allocator,
				CommandBuffer& commandBuffer, UploadContext& uploadContext, Queue& queue,
				VkImageAspectFlags aspectFlags) :
				_image(device, file, allocator, commandBuffer, uploadContext, queue),
				_imageView(device, _image, aspectFlags)
		{

		}

		Image _image;
		ImageView _imageView;
	};
	using TexturePtr = std::shared_ptr<Texture>;
}
#endif //CONCERTOGRAPHICS_TEXTURE_HPP
