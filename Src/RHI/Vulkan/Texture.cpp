//
// Created by arthur on 17/08/2022.
//

#include "Concerto/Graphics/RHI/Vulkan/VKRHITexture.hpp"
#include "Concerto/Graphics/Backend/Vulkan/Wrapper/Device.hpp"
#include "Concerto/Graphics/Backend/Vulkan/UploadContext.hpp"

namespace Concerto::Graphics::RHI
{
	VkRHITexture::VkRHITexture(Vk::Device& device, const std::string& file, Vk::CommandBuffer& commandBuffer, Vk::UploadContext& uploadContext, Vk::Queue& queue, VkImageAspectFlags aspectFlags) :
		_image(device, file, commandBuffer, uploadContext, queue),
		_imageView(device, _image, aspectFlags)
	{
		_image.SetDebugName("lol");
	}

	const Vk::Image& VkRHITexture::GetImage() const
	{
		return _image;
	}

	const Vk::ImageView& VkRHITexture::GetImageView() const
	{
		return _imageView;
	}

	VkRHITextureView::VkRHITextureView(const Vk::ImageView& image) :
		_imageView(&image)
	{
	}

	const Vk::ImageView& VkRHITextureView::GetImageView() const
	{
		return *_imageView;
	}
}
