//
// Created by arthur on 13/09/2022.
//

#ifndef CONCERTOGRAPHICS_QUEUE_HPP
#define CONCERTOGRAPHICS_QUEUE_HPP

#include "Frame.hpp"
#include "wrapper/Swapchain.hpp"
#include "wrapper/Object.hpp"

namespace Concerto::Graphics::Wrapper
{
	class Device;

	class Queue : public Object<VkQueue>
	{
	public:
		enum class Type
		{
			Compute, Graphics, Transfer
		};

		explicit Queue(Device& device, std::uint32_t queueFamilyIndex);

		Queue(Queue&&) = default;

		Queue(const Queue&) = delete;

		Queue& operator=(Queue&&) = default;

		Queue& operator=(const Queue&) = delete;

		[[nodiscard]] std::uint32_t GetFamilyIndex() const;

		void Submit(const FrameData& frame);

		void Present(const FrameData& frame, Swapchain& swapchain, std::uint32_t swapchainImageIndex);

	private:
		std::uint32_t _queueFamilyIndex;
	};

}

#endif //CONCERTOGRAPHICS_QUEUE_HPP
