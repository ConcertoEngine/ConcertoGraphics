//
// Created by arthur on 12/06/22.
//

#ifndef CONCERTO_GRAPHICS_FRAMEBUFFER_HPP
#define CONCERTO_GRAPHICS_FRAMEBUFFER_HPP

#include <vector>

#include "Concerto/Graphics/Backend/Vulkan/Defines.hpp"
#include "Concerto/Graphics/Backend/Vulkan/Wrapper/Object.hpp"

namespace Concerto::Graphics::Vk
{
	class Device;

	class RenderPass;

	class ImageView;

	/**
	* @class FrameBuffer
	* @brief This class represents a VkFramebuffer.
	* A frame buffer is a collection of images that are used as the destination for rendering.
	*
	* This class is a Wrapper around the VkFramebuffer handle and provides
	* a simpler interface for interacting with frame buffers.
	*/
	class CONCERTO_GRAPHICS_VULKAN_BACKEND_API FrameBuffer : public Object<VkFramebuffer>
	{
	public:
		/**
		* @brief Constructs a new frame buffer.
		*
		* @param device The device to create the frame buffer on.
		* @param renderPass The render pass the frame buffer will be used with.
		* @param attachments
		* @param extent The dimensions of the frame buffer.
		*/
		FrameBuffer(Device& device, const RenderPass& renderPass, const std::vector<VkImageView>& attachments, VkExtent2D extent);

		~FrameBuffer();

		FrameBuffer(FrameBuffer&&) noexcept = default;

		FrameBuffer(const FrameBuffer&) = delete;

		FrameBuffer& operator=(FrameBuffer&&) noexcept = default;

		FrameBuffer& operator=(const FrameBuffer&) = delete;
	};

} // Concerto::Graphics::Vk

#endif //CONCERTO_GRAPHICS_FRAMEBUFFER_HPP
