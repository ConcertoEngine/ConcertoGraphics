//
// Created by arthur on 18/09/2022.
//

#ifndef CONCERTOGRAPHICS_TEXTURE_HPP
#define CONCERTOGRAPHICS_TEXTURE_HPP

#include <memory>
#include <string>

#include <vulkan/vulkan.h>

#include "Concerto/Graphics/Vulkan/Wrapper/Image.hpp"
#include "Concerto/Graphics/Vulkan/Wrapper/ImageView.hpp"


namespace Concerto::Graphics
{
	class Device;
	class CommandBuffer;
	class UploadContext;
	class Queue;
	
	class Texture
	{
	public:
		Texture(Device& device,
				const std::string& file,
				CommandBuffer& commandBuffer,
				UploadContext& uploadContext,
				Queue& queue,
				VkImageAspectFlags aspectFlags);

		Image _image;
		ImageView _imageView;
	};
	using TexturePtr = std::shared_ptr<Texture>;
}
#endif //CONCERTOGRAPHICS_TEXTURE_HPP
