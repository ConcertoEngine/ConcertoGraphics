//
// Created by arthur on 18/09/2022.
//

#include <utility>
#include "wrapper/ImageView.hpp"
#include "wrapper/VulkanInitializer.hpp"

namespace Concerto::Graphics::Wrapper
{
	ImageView::ImageView(Image& image, VkImageAspectFlags aspectFlags, VkDevice device) : _device(device)
	{
		auto imageInfo = VulkanInitializer::ImageViewCreateInfo(image.GetFormat(), *image.Get(), aspectFlags);
		if (vkCreateImageView(device, &imageInfo, nullptr, &_imageView) != VK_SUCCESS)
		{
			throw std::runtime_error("failed to create image view");
		}
	}

	ImageView::ImageView(ImageView&& other) noexcept
	{
		_imageView = std::exchange(other._imageView, VK_NULL_HANDLE);
		_device = std::exchange(other._device, nullptr);
	}

	ImageView& ImageView::operator=(ImageView&& other) noexcept
	{
		_imageView = std::exchange(other._imageView, VK_NULL_HANDLE);
		_device = std::exchange(other._device, nullptr);
		return *this;
	}

	ImageView::~ImageView()
	{
		if (_device != nullptr && _imageView != VK_NULL_HANDLE)
			vkDestroyImageView(_device, _imageView, nullptr);
		_imageView = VK_NULL_HANDLE;
		_device = nullptr;
	}

	VkImageView* ImageView::Get()
	{
		assert(_imageView != VK_NULL_HANDLE);
		return &_imageView;
	}
}