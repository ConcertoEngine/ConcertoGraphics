//
// Created by arthur on 17/09/2022.
//

#include <format>

#include "Concerto/Graphics/Backend/Vulkan/Wrapper/Device.hpp"
#include "Concerto/Graphics/Backend/Vulkan/Wrapper/Buffer.hpp"
#include "Concerto/Graphics/Backend/Vulkan/Wrapper/Image.hpp"
#include "Concerto/Graphics/Backend/Vulkan/Wrapper/CommandBuffer.hpp"
#include "Concerto/Graphics/Backend/Vulkan/Wrapper/Queue.hpp"
#include "Concerto/Graphics/Backend/Vulkan/Wrapper/VulkanInitializer.hpp"
#include "Concerto/Graphics/Backend/Vulkan/UploadContext.hpp"

namespace cct::gfx::vk
{
	Image::Image(Device& device, VkExtent2D extent, VkFormat format, VkImageUsageFlags usageFlags) :
		Object(device),
		_isAllocated(true),
		_imageFormat(format),
		_extent(extent)
	{
		VkExtent3D depthImageExtent = {
				extent.width,
				extent.height,
				1
		};
		VkImageCreateInfo imageCreateInfo = VulkanInitializer::ImageCreateInfo(format, usageFlags, depthImageExtent);
		VmaAllocationCreateInfo imageAllocInfo = {};
		imageAllocInfo.usage = VMA_MEMORY_USAGE_GPU_ONLY;
		imageAllocInfo.requiredFlags = static_cast<VkMemoryPropertyFlags>(VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);

		_lastResult = vmaCreateImage(*device.GetAllocator().Get(), &imageCreateInfo, &imageAllocInfo, &_handle, &_allocation, nullptr);
		CCT_ASSERT(_lastResult == VK_SUCCESS, "ConcertoGraphics: vmaCreateImage failed VkResult={}", static_cast<int>(_lastResult));
	}

	Image::~Image()
	{
		if (IsNull())
			return;
		if (!_isAllocated)
			return;
		vmaDestroyImage(*_device->GetAllocator().Get(), _handle, _allocation);
	}

	Image::Image(Image&& image) noexcept :
		Object(std::move(image)),
		_isAllocated(std::exchange(image._isAllocated, false)),
		_imageFormat(std::exchange(image._imageFormat, {})),
		_allocation(std::exchange(image._allocation, nullptr)),
		_extent(std::exchange(image._extent, {}))
	{
	}

	Image& Image::operator=(Image&& image) noexcept
	{
		std::swap(_isAllocated, image._isAllocated);
		std::swap(_imageFormat, image._imageFormat);
		std::swap(_allocation, image._allocation);
		std::swap(_extent, image._extent);
		Object::operator=(std::move(image));
		return *this;
	}

	Image::Image(Device& device, VkExtent2D extent, VkImage image, VkFormat imageFormat) :
		Object(device),
		_isAllocated(false),
		_imageFormat(imageFormat),
		_extent(extent)
	{
		_handle = image;
	}

	VkFormat Image::GetFormat() const
	{
		return _imageFormat;
	}

	VkExtent2D Image::GetExtent() const
	{
		return _extent;
	}
}
