//
// Created by arthur on 25/10/2022.
//

#ifndef CONCERTOGRAPHICS_DEVICE_HPP
#define CONCERTOGRAPHICS_DEVICE_HPP

#include <cstdint>
#include <span>
#include "vulkan/vulkan.h"
#include "Queue.hpp"

namespace Concerto::Graphics::Wrapper
{
	class PhysicalDevice;

	/**
	* @class Device
	* @brief This class represents a device in the Vulkan API.
	* A device is a virtual representation of a physical device,
	* such as a GPU, and provides access to the resources of that physical device.
	*/
	class Device
	{
	public:
		Device() = default;

		/**
		* @brief Constructs a new device from a physical device and a list of extensions.
		*
		* @param physicalDevice The physical device to create the device on.
		* @param extensions The list of extensions to enable for the device.
		*/
		explicit Device(PhysicalDevice& physicalDevice, std::span<const char*> extensions);

		/**
		* @brief Retrieves the queue family index for a given queue type.
		*
		* @param queueType The type of queue to retrieve the index for.
		*
		* @return The index of the queue family for the given queue type.
		*/
		std::uint32_t GetQueueFamilyIndex(Queue::Type queueType);

		/**
		* @brief Retrieves the queue family index for a given queue flag.
		*
		* @param queueFlag The flag of queue to retrieve the index for.
		*
		* @return The index of the queue family for the given queue flag.
		*/
		std::uint32_t GetQueueFamilyIndex(std::uint32_t queueFlag);

		/**
		* @brief Retrieves a queue of a given type.
		*
		* @param queueType The type of queue to retrieve.
		*
		* @return The queue of the given type.
		*/
		Queue GetQueue(Queue::Type queueType);

		/**
		* @brief Retrieves a pointer to the underlying VkDevice handle.
		*
		* @return A pointer to the underlying VkDevice handle.
		*/
		[[nodiscard]] VkDevice* Get();

		/**
		* @brief Waits for the device to become idle.
		*/
		void WaitIdle() const;

	private:
		PhysicalDevice* _physicalDevice;
		VkDevice _device;
	};

} // Concerto::Graphics::Wrapper

#endif //CONCERTOGRAPHICS_DEVICE_HPP
