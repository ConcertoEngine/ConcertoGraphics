//
// Created by arthur on 17/08/2022.
//

#include "Concerto/Graphics/RHI/Vulkan/VKRHITexture.hpp"
#include "Concerto/Graphics/Backend/Vulkan/Wrapper/Device.hpp"
#include "Concerto/Graphics/Backend/Vulkan/UploadContext.hpp"

namespace cct::gfx::rhi
{
	VkRHITexture::VkRHITexture(vk::Device& device, const std::string& file, vk::CommandBuffer& commandBuffer, vk::UploadContext& uploadContext, vk::Queue& queue, VkImageAspectFlags aspectFlags) :
		_image(device, file, commandBuffer, uploadContext, queue),
		_imageView(device, _image, aspectFlags)
	{
		_image.SetDebugName("lol");
	}

	const vk::Image& VkRHITexture::GetImage() const
	{
		return _image;
	}

	const vk::ImageView& VkRHITexture::GetImageView() const
	{
		return _imageView;
	}

	VkRHITextureView::VkRHITextureView(const vk::ImageView& image) :
		_imageView(&image)
	{
	}

	const vk::ImageView& VkRHITextureView::GetImageView() const
	{
		return *_imageView;
	}
}
