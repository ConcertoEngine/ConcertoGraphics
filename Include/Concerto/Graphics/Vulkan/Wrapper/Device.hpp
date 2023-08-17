//
// Created by arthur on 25/10/2022.
//

#ifndef CONCERTOGRAPHICS_DEVICE_HPP
#define CONCERTOGRAPHICS_DEVICE_HPP

#include <cstdint>
#include <span>

#include <vulkan/vulkan.h>
#include <Concerto/Core/Types.hpp>

#include "Concerto/Graphics/Vulkan/Wrapper/Queue.hpp"
#include "Concerto/Graphics/Vulkan/Wrapper/Allocator.hpp"

namespace Concerto::Graphics
{
	class PhysicalDevice;
	class Instance;

	/**
	* @class Device
	* @brief This class represents a device in the Vulkan API.
	* A device is a virtual representation of a physical device,
	* such as a GPU, and provides access to the resources of that physical device.
	*/
	class CONCERTO_PUBLIC_API Device
	{
	public:
		Device() = delete;

		/**
		* @brief Constructs a new device from a physical device and a list of extensions.
		*
		* @param physicalDevice The physical device to create the device on.
		* @param extensions The list of extensions to enable for the device.
		*/
		Device(PhysicalDevice& physicalDevice, Instance& instance);

		/**
		* @brief Retrieves the queue family index for a given queue type.
		*
		* @param queueType The type of queue to retrieve the index for.
		*
		* @return The index of the queue family for the given queue type.
		*/
		UInt32 GetQueueFamilyIndex(Queue::Type queueType);

		/**
		* @brief Retrieves the queue family index for a given queue flag.
		*
		* @param queueFlag The flag of queue to retrieve the index for.
		*
		* @return The index of the queue family for the given queue flag.
		*/
		UInt32 GetQueueFamilyIndex(UInt32 queueFlag);

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


		PhysicalDevice& GetPhysicalDevice();

		Allocator& GetAllocator();

	private:
		PhysicalDevice* _physicalDevice;
		VkDevice _device;
		Allocator _allocator;
	};

} // Concerto::Graphics::Wrapper

#endif //CONCERTOGRAPHICS_DEVICE_HPP
