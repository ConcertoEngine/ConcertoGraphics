//
// Created by arthur on 18/09/2022.
//

#ifndef CONCERTO_GRAPHICS_TEXTURE_HPP
#define CONCERTO_GRAPHICS_TEXTURE_HPP

#include <memory>
#include <string>

#include <vulkan/vulkan.h>

#include "Concerto/Graphics/Backend/Vulkan/Wrapper/Image.hpp"
#include "Concerto/Graphics/Backend/Vulkan/Wrapper/ImageView.hpp"


namespace Concerto::Graphics::Vk
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
#endif //CONCERTO_GRAPHICS_TEXTURE_HPP
