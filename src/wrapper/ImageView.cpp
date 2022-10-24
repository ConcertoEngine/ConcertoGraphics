//
// Created by arthur on 18/09/2022.
//

#include "wrapper/ImageView.hpp"
#include "wrapper/VulkanInitializer.hpp"

namespace Concerto::Graphics::Wrapper
{
	ImageView::ImageView(AllocatedImage& image, VkImageAspectFlags aspectFlags, VkDevice device) : _device(device)
	{
		auto imageInfo = VulkanInitializer::ImageViewCreateInfo(image.imageFormat, image._image, aspectFlags);
		if (vkCreateImageView(device, &imageInfo, nullptr, &_imageView) != VK_SUCCESS)
		{
			throw std::runtime_error("failed to create image view");
		}
	}

	VkImageView* ImageView::Get()
	{
		assert(_imageView != VK_NULL_HANDLE);
		return &_imageView;
	}

	ImageView::~ImageView()
	{
		vkDestroyImageView(_device, _imageView, nullptr);
		_imageView = VK_NULL_HANDLE;
	}
}