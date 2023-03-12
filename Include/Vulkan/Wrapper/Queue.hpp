//
// Created by arthur on 13/09/2022.
//

#ifndef CONCERTOGRAPHICS_QUEUE_HPP
#define CONCERTOGRAPHICS_QUEUE_HPP

#include <Concerto/Core/Types.hpp>
#include "Frame.hpp"
#include "Swapchain.hpp"
#include "Object.hpp"

namespace Concerto::Graphics::Wrapper
{
	class Device;

	/**
	* @class Queue
	* @brief Represents a queue for a specific queue family on a device.
	*
	*  A queue is a representation of a queue on a device, which can be used to submit commands to the device.
	*  Queue objects are associated with a specific queue family on the device, which determines the types of operations that can be performed on the queue.
	*
	*  The Queue class provides an interface to submit commands to the device and present images to the swapchain.
	*
	*/
	class CONCERTO_PUBLIC_API Queue : public Object<VkQueue>
	{
	 public:
		enum class Type
		{
			Compute,  /**< Compute queue for parallel computation. */
			Graphics, /**< Graphics queue for rendering operations. */
			Transfer  /**< Transfer queue for memory operations. */
		};

		/**
		* @brief Constructs a new Queue object.
		*
		* @param device The device that the queue will be associated with.
		* @param queueFamilyIndex The index of the queue family that the queue will be associated with.
		*/
		explicit Queue(Device& device, std::uint32_t queueFamilyIndex);

		Queue(Queue&&) = default;

		Queue(const Queue&) = delete;

		Queue& operator=(Queue&&) = default;

		Queue& operator=(const Queue&) = delete;

		/**
		* @brief Gets the index of the queue family that the queue is associated with.
		*
		* @return The index of the queue family.
		*/
		[[nodiscard]] std::uint32_t GetFamilyIndex() const;

		/**
		* @brief Submits a set of commands to the queue for execution.
		*
		* @param frame The frame data that contains the command buffers to submit.
		*/
		void Submit(const FrameData& frame);

		/**
		* @brief Presents an image from the swapchain to the screen.
		*
		* @param frame The frame data that contains the semaphores to use for presentation.
		* @param swapchain The swapchain that the image will be presented from.
		* @param swapchainImageIndex The index of the image in the swapchain to present.
		*
		* @return true if the presentation was successful, false otherwise.
		*/
		bool Present(const FrameData& frame, Swapchain& swapchain, std::uint32_t swapchainImageIndex);

	 private:
		std::uint32_t _queueFamilyIndex;
	};
} // namespace Concerto::Graphics::Wrapper

#endif //CONCERTOGRAPHICS_QUEUE_HPP
