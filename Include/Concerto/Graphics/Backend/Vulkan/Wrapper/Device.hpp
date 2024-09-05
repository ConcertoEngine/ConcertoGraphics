//
// Created by arthur on 25/10/2022.
//

#ifndef CONCERTO_GRAPHICS_DEVICE_HPP
#define CONCERTO_GRAPHICS_DEVICE_HPP

#include <cstdint>
#include <span>
#include <unordered_map>
#include <memory>

#include <vulkan/vulkan.h>
#include "Concerto/Graphics/Backend/Vulkan/Defines.hpp"

#include "Concerto/Graphics/Backend/Vulkan/Wrapper/Queue.hpp"
#include "Concerto/Graphics/Backend/Vulkan/Wrapper/Allocator.hpp"

namespace Concerto::Graphics::Vk
{
	class PhysicalDevice;
	class Instance;
	class UploadContext;

	/**
	* @class Device
	* @brief This class represents a device in the Vulkan API.
	* A device is a virtual representation of a physical device,
	* such as a GPU, and provides access to the resources of that physical device.
	*/
	class CONCERTO_GRAPHICS_VULKAN_BACKEND_API Device
	{
	public:
		Device() = delete;

		/**
		* @brief Constructs a new device from a physical device and a list of extensions.
		*
		* @param physicalDevice The physical device to create the device on.
		* @param instance The instance to create the device on.
		*/
		Device(PhysicalDevice& physicalDevice, Instance& instance);

		/**
		* @brief Retrieves the queue family index for a given queue type.
		*
		* @param queueType The type of queue to retrieve the index for.
		*
		* @return The index of the queue family for the given queue type.
		*/
		[[nodiscard]] UInt32 GetQueueFamilyIndex(Queue::Type queueType) const;

		/**
		* @brief Retrieves the queue family index for a given queue flag.
		*
		* @param queueFlag The flag of queue to retrieve the index for.
		*
		* @return The index of the queue family for the given queue flag.
		*/
		[[nodiscard]] UInt32 GetQueueFamilyIndex(UInt32 queueFlag) const;

		/**
		* @brief Retrieves a queue of a given type.
		*
		* @param queueType The type of queue to retrieve.
		*
		* @return The queue of the given type.
		*/
		Queue& GetQueue(Queue::Type queueType);

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

		/**
		* @brief Update the contents of a descriptor set object
		*/
		void UpdateDescriptorSetsWrite(std::span<VkWriteDescriptorSet> descriptorWrites) const;

		/**
		* @brief Update the contents of a descriptor set object
		*/
		void UpdateDescriptorSetWrite(VkWriteDescriptorSet descriptorWrite);

		void SetObjectName(UInt64 object, std::string_view name);

		PhysicalDevice& GetPhysicalDevice() const;

		Allocator& GetAllocator() const;

	private:
		void CreateAllocator(Instance& instance);

		PhysicalDevice& _physicalDevice;
		VkDevice _device;
		std::unique_ptr<Allocator> _allocator;
		std::unordered_map<Queue::Type, Queue> _queues;
	};

} // Concerto::Graphics::Vk

#endif //CONCERTO_GRAPHICS_DEVICE_HPP
