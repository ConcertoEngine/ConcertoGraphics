//
// Created by arthur on 13/09/2022.
//

#ifndef CONCERTOGRAPHICS_QUEUE_HPP
#define CONCERTOGRAPHICS_QUEUE_HPP

#include "VkBootstrap.h"
#include "Frame.hpp"
#include "wrapper/Swapchain.hpp"

namespace Concerto::Graphics::Wrapper
{
	class Queue
	{
	public:
		Queue(const vkb::Device& device);
		std::uint32_t GetFamilyIndex() const;
		void Submit(const FrameData& frame);
		void Present(const FrameData& frame, Swapchain& swapchain, std::uint32_t swapchainImageIndex);
		VkQueue *Get();
	private:
		vkb::Device _device;
		VkQueue _queue;
	};

}

#endif //CONCERTOGRAPHICS_QUEUE_HPP
