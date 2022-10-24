//
// Created by arthur on 17/09/2022.
//
#define STB_IMAGE_IMPLEMENTATION

#include <stb_image.h>
#include "wrapper/AllocatedBuffer.hpp"
#include "wrapper/AllocatedImage.hpp"
#include "wrapper/CommandBuffer.hpp"
#include "UploadContext.hpp"
#include "wrapper/Queue.hpp"
#include "Utils.hpp"

namespace Concerto::Graphics::Wrapper
{
	AllocatedImage::AllocatedImage(VkExtent2D extent, VkFormat depthFormat, Allocator& allocator) : imageFormat(
			depthFormat)
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

		if (vmaCreateImage(allocator._allocator, &dimg_info, &dimg_allocinfo, &_image, &_allocation, nullptr) !=
			VK_SUCCESS)
		{
			throw std::runtime_error("Failed to create image");
		}
	}

	AllocatedImage::AllocatedImage(const std::string& file, VkExtent2D extent, Allocator& allocator,
			CommandBuffer& commandBuffer, UploadContext& uploadContext, Queue& queue) :
			imageFormat(VK_FORMAT_R8G8B8A8_SRGB)
	{
		int textureWidth, textureHeight, textureChannels;
		stbi_uc* pixels = stbi_load(file.c_str(), &textureWidth, &textureHeight, &textureChannels, STBI_rgb_alpha);
		if (!pixels)
		{
			throw std::runtime_error("Failed to load texture image");
		}

		void* pixelPtr = pixels;
		VkDeviceSize imageSize = textureWidth * textureHeight * 4;
		AllocatedBuffer stagingBuffer(allocator, imageSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
				VMA_MEMORY_USAGE_CPU_ONLY);
		MapAndCopy(allocator, stagingBuffer, pixelPtr, imageSize);
		stbi_image_free(pixels);

		VkExtent3D imageExtent = {
				extent.width,
				extent.height,
				1
		};
		VkImageCreateInfo dimg_info = VulkanInitializer::ImageCreateInfo(VK_FORMAT_R8G8B8A8_SRGB,
				VK_IMAGE_USAGE_SAMPLED_BIT | VK_IMAGE_USAGE_TRANSFER_DST_BIT, imageExtent);
		VmaAllocationCreateInfo dimg_allocinfo = {};
		dimg_allocinfo.usage = VMA_MEMORY_USAGE_GPU_ONLY;

		if (vmaCreateImage(allocator._allocator, &dimg_info, &dimg_allocinfo, &_image, &_allocation, nullptr) !=
			VK_SUCCESS)
		{
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
					imageBarrier_toTransfer.image = _image;
					imageBarrier_toTransfer.subresourceRange = range;

					imageBarrier_toTransfer.srcAccessMask = 0;
					imageBarrier_toTransfer.dstAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;

					//barrier the image into the transfer-receive layout
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
					vkCmdCopyBufferToImage(cb.Get(), stagingBuffer._buffer, _image,
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
}