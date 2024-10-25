//
// Created by arthur on 18/09/2022.
//

#include <utility>

#include "Concerto/Graphics/Backend/Vulkan/Wrapper/Device.hpp"
#include "Concerto/Graphics/Backend/Vulkan/Wrapper/ImageView.hpp"

#include <Concerto/Core/Assert.hpp>

#include "Concerto/Graphics/Backend/Vulkan/Wrapper/Image.hpp"
#include "Concerto/Graphics/Backend/Vulkan/Wrapper/VulkanInitializer.hpp"

namespace Concerto::Graphics::Vk
{
	ImageView::ImageView(Device& device, Image& image, VkImageAspectFlags aspectFlags) :
		Object(device),
		_image(&image)
	{
		auto imageInfo = VulkanInitializer::ImageViewCreateInfo(image.GetFormat(), *image.Get(), aspectFlags);
		_lastResult = _device->vkCreateImageView(*_device->Get(), &imageInfo, nullptr, &_handle);
		CONCERTO_ASSERT(_lastResult == VK_SUCCESS, "ConcertoGraphics: vkCreateImageView failed VkResult={}", static_cast<int>(_lastResult));
		Logger::Info("{}, {}x{}", reinterpret_cast<void*>(_handle), image.GetExtent().width, image.GetExtent().height);
	}

	ImageView::~ImageView()
	{
		if (IsNull())
			return;
		_device->WaitIdle();
		_device->vkDestroyImageView(*_device->Get(), _handle, nullptr);
	}
}
