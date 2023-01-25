//
// Created by arthur on 18/09/2022.
//

#include <utility>

#include "wrapper/Device.hpp"
#include "wrapper/ImageView.hpp"
#include "wrapper/VulkanInitializer.hpp"

namespace Concerto::Graphics::Wrapper
{
	ImageView::ImageView(Device& device, Image& image, VkImageAspectFlags aspectFlags) : Object<VkImageView>(device, [this](Device &device, VkImageView handle){
		device.WaitIdle();
		vkDestroyImageView(*device.Get(), handle, nullptr);
	})
	{
		auto imageInfo = VulkanInitializer::ImageViewCreateInfo(image.GetFormat(), *image.Get(), aspectFlags);
		if (vkCreateImageView(*_device->Get(), &imageInfo, nullptr, &_handle) != VK_SUCCESS)
		{
			throw std::runtime_error("failed to create image view");
		}
	}
}