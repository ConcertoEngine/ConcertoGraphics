//
// Created by arthur on 13/09/2022.
//

#ifndef CONCERTO_GRAPHICS_QUEUE_HPP
#define CONCERTO_GRAPHICS_QUEUE_HPP

#include "Concerto/Graphics/Defines.hpp"
#include "Concerto/Graphics/Vulkan/Wrapper/Object.hpp"

namespace Concerto::Graphics
{
	class Device;
	class Swapchain;
	class FrameData;

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
	class CONCERTO_GRAPHICS_API Queue : public Object<VkQueue>
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
		explicit Queue(Device& device, UInt32 queueFamilyIndex);

		~Queue() = default;

		Queue(Queue&&) = default;

		Queue(const Queue&) = delete;

		Queue& operator=(Queue&&) = default;

		Queue& operator=(const Queue&) = delete;

		/**
		* @brief Gets the index of the queue family that the queue is associated with.
		*
		* @return The index of the queue family.
		*/
		[[nodiscard]] UInt32 GetFamilyIndex() const;

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
		bool Present(const FrameData& frame, Swapchain& swapchain, UInt32 swapchainImageIndex);

	 private:
		UInt32 _queueFamilyIndex;
	};
} // namespace Concerto::Graphics

#endif //CONCERTO_GRAPHICS_QUEUE_HPP
