//
// Created by arthur on 17/09/2022.
//

#define STB_IMAGE_IMPLEMENTATION

#include <format>
#include <stb_image.h>

#include "Concerto/Graphics/Backend/Vulkan/Wrapper/Device.hpp"
#include "Concerto/Graphics/Backend/Vulkan/Wrapper/Buffer.hpp"
#include "Concerto/Graphics/Backend/Vulkan/Wrapper/Image.hpp"
#include "Concerto/Graphics/Backend/Vulkan/Wrapper/CommandBuffer.hpp"
#include "Concerto/Graphics/Backend/Vulkan/Wrapper/Queue.hpp"
#include "Concerto/Graphics/Backend/Vulkan/Wrapper/VulkanInitializer.hpp"
#include "Concerto/Graphics/Backend/Vulkan/UploadContext.hpp"

namespace cct::gfx::vk
{
	Image::Image(Device& device, VkExtent2D extent, VkFormat depthFormat) :
		Object(device),
		_isAllocated(true),
		_imageFormat(depthFormat),
		_extent(extent)
	{
		VkExtent3D depthImageExtent = {
				extent.width,
				extent.height,
				1
		};
		VkImageCreateInfo imageCreateInfo = VulkanInitializer::ImageCreateInfo(depthFormat, VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT, depthImageExtent);
		VmaAllocationCreateInfo imageAllocInfo = {};
		imageAllocInfo.usage = VMA_MEMORY_USAGE_GPU_ONLY;
		imageAllocInfo.requiredFlags = static_cast<VkMemoryPropertyFlags>(VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);

		_lastResult = vmaCreateImage(*device.GetAllocator().Get(), &imageCreateInfo, &imageAllocInfo, &_handle, &_allocation, nullptr);
		CCT_ASSERT(_lastResult == VK_SUCCESS, "ConcertoGraphics: vmaCreateImage failed VkResult={}", static_cast<int>(_lastResult));
	}

	Image::Image(Device& device, const std::string& file, CommandBuffer& commandBuffer, UploadContext& uploadContext, Queue& queue) :
		Object(device),
		_isAllocated(true),
		_imageFormat(VK_FORMAT_R8G8B8A8_SRGB),
		_extent()
	{
		int textureWidth, textureHeight, textureChannels;
		stbi_uc* pixels = stbi_load(file.c_str(), &textureWidth, &textureHeight, &textureChannels, STBI_rgb_alpha);
		if (!pixels)
		{
			CCT_ASSERT_FALSE("ConcertoGraphics: Failed to load texture '{}'", file);
			return;
		}

		void* pixelPtr = pixels;
		VkDeviceSize imageSize = textureWidth * textureHeight * 4;
		Buffer stagingBuffer(device.GetAllocator(), imageSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VMA_MEMORY_USAGE_CPU_ONLY, true);
		stagingBuffer.Copy(pixelPtr, imageSize, 0);

		_extent.width = static_cast<uint32_t>(textureWidth);
		_extent.height = static_cast<uint32_t>(textureHeight);
		VkExtent3D extent3D = {
			.width = _extent.width,
			.height = _extent.height,
			.depth = 1
		};
		VkImageCreateInfo imageCreateInfo = VulkanInitializer::ImageCreateInfo(VK_FORMAT_R8G8B8A8_SRGB, VK_IMAGE_USAGE_SAMPLED_BIT | VK_IMAGE_USAGE_TRANSFER_DST_BIT, extent3D);
		VmaAllocationCreateInfo imageAllocInfo = {};
		imageAllocInfo.usage = VMA_MEMORY_USAGE_GPU_ONLY;

		const VkResult result = vmaCreateImage(*device.GetAllocator().Get(), &imageCreateInfo, &imageAllocInfo, &_handle, &_allocation, nullptr);
		CCT_ASSERT(result == VK_SUCCESS, "ConcertoGraphics: vmaCreateImage failed VKResult={}", static_cast<int>(result));

		commandBuffer.ImmediateSubmit(uploadContext._uploadFence, uploadContext._commandPool, queue,
				[&](CommandBuffer& cb)
				{
					VkImageSubresourceRange range;
					range.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
					range.baseMipLevel = 0;
					range.levelCount = 1;
					range.baseArrayLayer = 0;
					range.layerCount = 1;

					VkImageMemoryBarrier imageBarrier_toTransfer = {};
					imageBarrier_toTransfer.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;

					imageBarrier_toTransfer.oldLayout = VK_IMAGE_LAYOUT_UNDEFINED;
					imageBarrier_toTransfer.newLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
					imageBarrier_toTransfer.image = _handle;
					imageBarrier_toTransfer.subresourceRange = range;

					imageBarrier_toTransfer.srcAccessMask = 0;
					imageBarrier_toTransfer.dstAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;

					//barrier the image into the Transfer-receive layout
					_device->vkCmdPipelineBarrier(*cb.Get(), VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT, VK_PIPELINE_STAGE_TRANSFER_BIT, 0,
							0, nullptr, 0, nullptr, 1, &imageBarrier_toTransfer);

					VkBufferImageCopy copyRegion = {};
					copyRegion.bufferOffset = 0;
					copyRegion.bufferRowLength = 0;
					copyRegion.bufferImageHeight = 0;

					copyRegion.imageSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
					copyRegion.imageSubresource.mipLevel = 0;
					copyRegion.imageSubresource.baseArrayLayer = 0;
					copyRegion.imageSubresource.layerCount = 1;
					copyRegion.imageExtent = extent3D;

					//copy the buffer into the image
					_device->vkCmdCopyBufferToImage(*cb.Get(), *stagingBuffer.Get(), _handle, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 1, &copyRegion);

					VkImageMemoryBarrier imageBarrier_toReadable = imageBarrier_toTransfer;

					imageBarrier_toReadable.oldLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
					imageBarrier_toReadable.newLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;

					imageBarrier_toReadable.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
					imageBarrier_toReadable.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;

					//barrier the image into the shader readable layout
					_device->vkCmdPipelineBarrier(*cb.Get(), VK_PIPELINE_STAGE_TRANSFER_BIT,
							VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT, 0, 0, nullptr, 0, nullptr, 1,
							&imageBarrier_toReadable);
				});
	}

	Image::~Image()
	{
		if (IsNull())
			return;
		if (!_isAllocated)
			return;
		vmaDestroyImage(*_device->GetAllocator().Get(), _handle, _allocation);
	}

	Image::Image(Image&& image) noexcept:
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
