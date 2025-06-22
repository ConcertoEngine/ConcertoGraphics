//
// Created by arthur on 17/08/2022.
//

#include "Concerto/Graphics/RHI/Vulkan/VKRHITexture.hpp"
#include "Concerto/Graphics/Backend/Vulkan/Wrapper/Device.hpp"
#include "Concerto/Graphics/Backend/Vulkan/UploadContext.hpp"
#include "Concerto/Graphics/RHI/Vulkan/Utils.hpp"

namespace cct::gfx::rhi
{
	VkRHITexture::VkRHITexture(vk::Device& device, PixelFormat format, Int32 width, Int32 height, VkImageAspectFlags aspectFlags) :
		_image(device, VkExtent2D{ static_cast<UInt32>(width), static_cast<UInt32>(height)}, Converters::ToVulkan(format), VK_IMAGE_USAGE_SAMPLED_BIT | VK_IMAGE_USAGE_TRANSFER_DST_BIT),
		_imageView(device, _image, aspectFlags)
	{
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
