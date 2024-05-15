//
// Created by arthur on 09/06/22.
//

#ifndef CONCERTO_GRAPHICS_IMAGE_HPP
#define CONCERTO_GRAPHICS_IMAGE_HPP

#include <string>

#include <vulkan/vulkan.h>
#include <vk_mem_alloc.h>

#include "Concerto/Graphics/Defines.hpp"
#include "Concerto/Graphics/Backend/Vulkan/Wrapper/Object.hpp"

namespace Concerto::Graphics::Vk
{
	class UploadContext;
	class CommandBuffer;
	class Queue;
	class Device;

	/**
	* @class Image
	* @brief This class represents an image in the Vulkan API.
	* An image is a two-dimensional array of pixels that can be used as a source or destination for rendering.
	*
	*/
	class CONCERTO_GRAPHICS_API Image : public Object<VkImage>
	{
	public:
		/**
		* @brief Constructs a new image from an existing image handle.
		*
		* @param device The device that the image was created on.
		* @param image The existing image handle.
		* @param imageFormat The format of the image.
		*/
		Image(Device& device, VkImage image, VkFormat imageFormat);

		/**
		* @brief Constructs a new image with the specified extent and format.
		*
		* @param device The device to create the image on.
		* @param extent The dimensions of the image.
		* @param depthFormat The format of the image.
		*/
		Image(Device& device, VkExtent2D extent, VkFormat depthFormat);

		/**
		* @brief Constructs a new image from a file.
		*
		* @param device The device to create the image on.
		* @param file The path to the image file.
		* @param commandBuffer The command buffer to use for uploading the image data.
		* @param uploadContext The upload context to use for uploading the image data.
		* @param queue The queue to use for uploading the image data.
		*/
		Image(Device& device, const std::string& file, CommandBuffer& commandBuffer, UploadContext& uploadContext, Queue& queue);

		~Image();

		Image(Image&&) = default;

		Image(const Image&) = delete;

		Image& operator=(Image&&) = default;

		Image& operator=(const Image&) = delete;

		/**
		* @brief Gets the format of the image.
		*
		* @return The format of the image.
		*/
		[[nodiscard]] VkFormat GetFormat() const;

	private:
		bool _isAllocated;
		VkFormat _imageFormat{};
		VmaAllocation _allocation{};
	};
}

#endif //CONCERTO_GRAPHICS_IMAGE_HPP