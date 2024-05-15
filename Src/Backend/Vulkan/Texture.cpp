//
// Created by arthur on 17/08/2022.
//

#include "Concerto/Graphics/Backend/Vulkan/Texture.hpp"
#include "Concerto/Graphics/Backend/Vulkan/Wrapper/Device.hpp"
#include "Concerto/Graphics/Backend/Vulkan/Wrapper/CommandBuffer.hpp"
#include "Concerto/Graphics/Backend/Vulkan/Wrapper/CommandBuffer.hpp"
#include "Concerto/Graphics/Backend/Vulkan/Wrapper/Queue.hpp"
#include "Concerto/Graphics/UploadContext.hpp"
#include "Concerto/Graphics/Backend/Vulkan/Wrapper/Allocator.hpp"

namespace Concerto::Graphics::Vk
{
	Texture::Texture(Device& device, const std::string& file, CommandBuffer& commandBuffer, UploadContext& uploadContext, Queue& queue,
			VkImageAspectFlags aspectFlags) :
			_image(device, file, commandBuffer, uploadContext, queue),
			_imageView(device, _image, aspectFlags)
	{

	}
}
