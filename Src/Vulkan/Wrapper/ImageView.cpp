//
// Created by arthur on 18/09/2022.
//

#include <utility>

#include "Concerto/Graphics/Vulkan/Wrapper/Device.hpp"
#include "Concerto/Graphics/Vulkan/Wrapper/ImageView.hpp"

#include <Concerto/Core/Assert.hpp>

#include "Concerto/Graphics/Vulkan/Wrapper/Image.hpp"
#include "Concerto/Graphics/Vulkan/Wrapper/VulkanInitializer.hpp"

namespace Concerto::Graphics
{
	ImageView::ImageView(Device& device, Image& image, VkImageAspectFlags aspectFlags) : Object(device)
	{
		auto imageInfo = VulkanInitializer::ImageViewCreateInfo(image.GetFormat(), *image.Get(), aspectFlags);
		if (vkCreateImageView(*_device->Get(), &imageInfo, nullptr, &_handle) != VK_SUCCESS)
		{
			CONCERTO_ASSERT_FALSE;
			throw std::runtime_error("failed to create image view");
		}
	}

	ImageView::~ImageView()
	{
		if (IsNull())
			return;
		_device->WaitIdle();
		vkDestroyImageView(*_device->Get(), _handle, nullptr);
	}
}
