//
// Created by arthur on 18/09/2022.
//

#include <utility>

#include "Vulkan/wrapper/Device.hpp"
#include "Vulkan/wrapper/ImageView.hpp"
#include "Vulkan/wrapper/VulkanInitializer.hpp"

namespace Concerto::Graphics
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