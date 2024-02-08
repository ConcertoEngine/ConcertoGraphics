//
// Created by arthur on 17/09/2022.
//

#define STB_IMAGE_IMPLEMENTATION

#include <stb_image.h>

#include "Concerto/Graphics/Vulkan/Wrapper/Buffer.hpp"
#include "Concerto/Graphics/Vulkan/Wrapper/Image.hpp"
#include "Concerto/Graphics/Vulkan/Wrapper/CommandBuffer.hpp"
#include "Concerto/Graphics/Vulkan/Wrapper/Queue.hpp"
#include "Concerto/Graphics/Vulkan/Utils.hpp"
#include "Concerto/Graphics/UploadContext.hpp"

namespace Concerto::Graphics
{
	Image::Image(Device& device, VkExtent2D extent, VkFormat depthFormat) :
		Object<VkImage>(device, [this](Device &device, VkImage handle)
			{
				if (!_isAllocated)
					return;
				vmaDestroyImage(*device.GetAllocator().Get(), handle, _allocation);
			}),
		_imageFormat(depthFormat),
		_isAllocated(true)
	{
		VkExtent3D depthImageExtent = {
				extent.width,
				extent.height,
				1
		};
		VkImageCreateInfo dimg_info = VulkanInitializer::ImageCreateInfo(depthFormat,
				VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT, depthImageExtent);
		VmaAllocationCreateInfo dimg_allocinfo = {};
		dimg_allocinfo.usage = VMA_MEMORY_USAGE_GPU_ONLY;
		dimg_allocinfo.requiredFlags = VkMemoryPropertyFlags(VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);

		if (vmaCreateImage(*device.GetAllocator().Get(), &dimg_info, &dimg_allocinfo, &_handle, &_allocation, nullptr) !=
			VK_SUCCESS)
		{
			CONCERTO_ASSERT_FALSE;
			throw std::runtime_error("Failed to create image");
		}
	}

	Image::Image(Device& device, const std::string& file, CommandBuffer& commandBuffer, UploadContext& uploadContext, Queue& queue) :
		Object<VkImage>(device, [this](Device& device, VkImage handle)
			{
				if (!_isAllocated)
					return;
				if (!IsNull())
					vmaDestroyImage(*device.GetAllocator().Get(), handle, _allocation);
			}),
		_imageFormat(VK_FORMAT_R8G8B8A8_SRGB),
		_isAllocated(true)
	{
		int textureWidth, textureHeight, textureChannels;
		stbi_uc* pixels = stbi_load(file.c_str(), &textureWidth, &textureHeight, &textureChannels, STBI_rgb_alpha);
		if (!pixels)
		{
			CONCERTO_ASSERT_FALSE;
			throw std::runtime_error("Failed to load texture image");
		}

		void* pixelPtr = pixels;
		VkDeviceSize imageSize = textureWidth * textureHeight * 4;
		Buffer stagingBuffer(device.GetAllocator(), imageSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VMA_MEMORY_USAGE_CPU_ONLY);
		stagingBuffer.Copy(pixelPtr, imageSize, 0);

		VkExtent3D imageExtent = {
				static_cast<uint32_t>(textureWidth),
				static_cast<uint32_t>(textureHeight),
				1
		};
		VkImageCreateInfo dimg_info = VulkanInitializer::ImageCreateInfo(VK_FORMAT_R8G8B8A8_SRGB,
			VK_IMAGE_USAGE_SAMPLED_BIT | VK_IMAGE_USAGE_TRANSFER_DST_BIT, imageExtent);
		VmaAllocationCreateInfo dimg_allocinfo = {};
		dimg_allocinfo.usage = VMA_MEMORY_USAGE_GPU_ONLY;

		if (vmaCreateImage(*device.GetAllocator().Get(), &dimg_info, &dimg_allocinfo, &_handle, &_allocation, nullptr) !=
			VK_SUCCESS)
		{
			CONCERTO_ASSERT_FALSE;
			throw std::runtime_error("Failed to create image");
		}
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
					vkCmdPipelineBarrier(cb.Get(), VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT, VK_PIPELINE_STAGE_TRANSFER_BIT, 0,
							0, nullptr, 0, nullptr, 1, &imageBarrier_toTransfer);

					VkBufferImageCopy copyRegion = {};
					copyRegion.bufferOffset = 0;
					copyRegion.bufferRowLength = 0;
					copyRegion.bufferImageHeight = 0;

					copyRegion.imageSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
					copyRegion.imageSubresource.mipLevel = 0;
					copyRegion.imageSubresource.baseArrayLayer = 0;
					copyRegion.imageSubresource.layerCount = 1;
					copyRegion.imageExtent = imageExtent;

					//copy the buffer into the image
					vkCmdCopyBufferToImage(cb.Get(), *stagingBuffer.Get(), _handle,
							VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 1, &copyRegion);

					VkImageMemoryBarrier imageBarrier_toReadable = imageBarrier_toTransfer;

					imageBarrier_toReadable.oldLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
					imageBarrier_toReadable.newLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;

					imageBarrier_toReadable.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
					imageBarrier_toReadable.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;

					//barrier the image into the shader readable layout
					vkCmdPipelineBarrier(cb.Get(), VK_PIPELINE_STAGE_TRANSFER_BIT,
							VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT, 0, 0, nullptr, 0, nullptr, 1,
							&imageBarrier_toReadable);
				});
	}

	Image::Image(Device& device, VkImage image, VkFormat imageFormat) :
		Object<VkImage>(device, [this](Device &device, VkImage handle)
			{
				if (!_isAllocated)
					return;
				vmaDestroyImage(*_device->GetAllocator().Get(), handle, _allocation);
			}),
		_isAllocated(false),
		_imageFormat(imageFormat)
	{
		_handle = image;
	}

	VkFormat Image::GetFormat() const
	{
		return _imageFormat;
	}
}