//
// Created by arthur on 13/09/2022.
//

#ifndef CONCERTO_GRAPHICS_QUEUE_HPP
#define CONCERTO_GRAPHICS_QUEUE_HPP

#include "Concerto/Graphics/Backend/Vulkan/Defines.hpp"
#include "Concerto/Graphics/Backend/Vulkan/Wrapper/Object.hpp"

namespace Concerto::Graphics::Vk
{
	class Device;
	class SwapChain;
	class FrameData;
	class CommandBuffer;
	class Semaphore;
	class Fence;

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
	class CONCERTO_GRAPHICS_VULKAN_BACKEND_API Queue : public Object<VkQueue>
	{
	 public:
		enum class Type
		{
			Compute,  /**< Compute queue for parallel computation. */
			Graphics, /**< Graphics queue for rendering operations. */
			Transfer  /**< Transfer queue for memory operations. */
		};

		explicit Queue(Device& device, UInt32 queueFamilyIndex);

		~Queue() = default;

		Queue(Queue&&) = default;

		Queue(const Queue&) = delete;

		Queue& operator=(Queue&&) = default;

		Queue& operator=(const Queue&) = delete;
		
		[[nodiscard]] UInt32 GetFamilyIndex() const;

		void Submit(const CommandBuffer& commandBuffer, const Semaphore& presentSemaphore, const Semaphore& renderSemaphore, const Fence& renderFence) const;

		bool Present(const Semaphore& renderSemaphore, SwapChain& swapchain, UInt32 swapchainImageIndex) const;

	 private:
		UInt32 _queueFamilyIndex;
	};
} // namespace Concerto::Graphics::Vk

#endif //CONCERTO_GRAPHICS_QUEUE_HPP
