//
// Created by arthur on 18/09/2022.
//

#include <utility>

#include "Concerto/Graphics/Backend/Vulkan/Wrapper/Device.hpp"
#include "Concerto/Graphics/Backend/Vulkan/Wrapper/ImageView.hpp"

#include <Concerto/Core/Assert.hpp>

#include "Concerto/Graphics/Backend/Vulkan/Wrapper/Image.hpp"
#include "Concerto/Graphics/Backend/Vulkan/Wrapper/VulkanInitializer.hpp"

namespace cct::gfx::vk
{
	ImageView::ImageView(Device& device, Image& image, VkImageAspectFlags aspectFlags) :
		Object(device),
		_image(&image)
	{
		auto imageInfo = VulkanInitializer::ImageViewCreateInfo(image.GetFormat(), *image.Get(), aspectFlags);
		_lastResult = _device->vkCreateImageView(*_device->Get(), &imageInfo, nullptr, &_handle);
		CCT_ASSERT(_lastResult == VK_SUCCESS, "ConcertoGraphics: vkCreateImageView failed VkResult={}", static_cast<int>(_lastResult));
	}

	ImageView::~ImageView()
	{
		if (IsNull())
			return;
		_device->WaitIdle();
		_device->vkDestroyImageView(*_device->Get(), _handle, nullptr);
	}
}
