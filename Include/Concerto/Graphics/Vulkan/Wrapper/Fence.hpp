//
// Created by arthur on 15/06/22.
//

#ifndef CONCERTO_GRAPHICS_FENCE_HPP
#define CONCERTO_GRAPHICS_FENCE_HPP

#include <vulkan/vulkan.hpp>
#include "Concerto/Graphics/Defines.hpp"

#include "Concerto/Graphics/Vulkan/Wrapper/Object.hpp"

namespace Concerto::Graphics
{
	class Device;

	/**
	* @class Fence
	* @brief This class represents a VkFence.
	* A fence is a synchronization primitive that can be used to insert a dependency from a queue
	* to the host, or to check the status of a queue operation.
	*
	* @note This class is not copyable, but it is movable.
	*/
	class CONCERTO_GRAPHICS_API Fence : public Object<VkFence>
	{
	public:
		/**
		* @brief Constructs a new fence on a given device.
		*
		* @param device The device to create the fence on.
		* @param signaled A flag indicating whether the fence should be signaled initially.
		*/
		explicit Fence(Device& device, bool signaled = true);

		Fence(Fence&&) = default;

		Fence(const Fence&) = delete;

		Fence& operator=(Fence&&) = default;

		Fence& operator=(const Fence&) = delete;

		/**
		* @brief Waits for the fence to be signaled.
		*
		* @param timeout The maximum time, in nanoseconds, to wait for the fence to be signaled.
		*/
		void Wait(UInt64 timeout);

		/**
		* @brief Resets the fence to an unsignaled state.
		*/
		void Reset();
	};
} // namespace Concerto::Graphics

#endif //CONCERTO_GRAPHICS_FENCE_HPP
