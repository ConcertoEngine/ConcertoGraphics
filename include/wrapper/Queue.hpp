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
	class Device;

	class Queue
	{
	public:
		enum class Type
		{
			Compute, Graphics, Transfer
		};

		explicit Queue(Device& device, std::uint32_t queueFamilyIndex);

		std::uint32_t GetFamilyIndex() const;

		void Submit(const FrameData& frame);

		void Present(const FrameData& frame, Swapchain& swapchain, std::uint32_t swapchainImageIndex);

		VkQueue* Get();

	private:
		Device& _device;
		VkQueue _queue;
		std::uint32_t _queueFamilyIndex;
	};

}

#endif //CONCERTOGRAPHICS_QUEUE_HPP
