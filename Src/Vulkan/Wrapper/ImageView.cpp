//
// Created by arthur on 18/09/2022.
//

#include <utility>

#include "Concerto/Graphics/Vulkan/Wrapper/Device.hpp"
#include "Concerto/Graphics/Vulkan/Wrapper/ImageView.hpp"
#include "Concerto/Graphics/Vulkan/Wrapper/VulkanInitializer.hpp"

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