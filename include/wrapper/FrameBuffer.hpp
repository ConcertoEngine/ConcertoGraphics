//
// Created by arthur on 12/06/22.
//

#ifndef CONCERTOGRAPHICS_FRAMEBUFFER_HPP
#define CONCERTOGRAPHICS_FRAMEBUFFER_HPP
#include <vector>
#include "vulkan/vulkan.hpp"
#include "Swapchain.hpp"
#include "RenderPass.hpp"
namespace Concerto::Graphics::Wrapper
{

	class FrameBuffer
	{
	public:
		FrameBuffer(VkDevice device, Swapchain &swapchain, RenderPass &renderPass);

		FrameBuffer(FrameBuffer&&) = default;

		FrameBuffer(const FrameBuffer&) = delete;

		FrameBuffer& operator=(FrameBuffer&&) = default;

		FrameBuffer& operator=(const FrameBuffer&) = delete;

		~FrameBuffer();

		VkFramebuffer operator[](std::size_t);
	private:
		std::vector<VkFramebuffer> _frameBuffers;
		Swapchain &_swapchain;
		RenderPass &_renderPass;
		VkDevice _device;
	};

} // Concerto::Graphics::Wrapper

#endif //CONCERTOGRAPHICS_FRAMEBUFFER_HPP
