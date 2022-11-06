//
// Created by arthur on 12/06/22.
//

#ifndef CONCERTOGRAPHICS_FRAMEBUFFER_HPP
#define CONCERTOGRAPHICS_FRAMEBUFFER_HPP

#include <vector>
#include "vulkan/vulkan.hpp"
#include "wrapper/Object.hpp"

namespace Concerto::Graphics::Wrapper
{
	class Device;

	class RenderPass;

	class ImageView;

	class FrameBuffer : public Object<VkFramebuffer>
	{
	public:
		FrameBuffer(Device& device, RenderPass& renderPass, ImageView& imageView, ImageView& depthImageView,
				VkExtent2D extent);

		FrameBuffer(FrameBuffer&&) noexcept = default;

		FrameBuffer(const FrameBuffer&) = delete;

		FrameBuffer& operator=(FrameBuffer&&) noexcept = default;

		FrameBuffer& operator=(const FrameBuffer&) = delete;
	};

} // Concerto::Graphics::Wrapper

#endif //CONCERTOGRAPHICS_FRAMEBUFFER_HPP
