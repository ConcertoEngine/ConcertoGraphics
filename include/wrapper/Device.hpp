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

	class Device
	{
	public:
		Device() = default;

		explicit Device(PhysicalDevice& physicalDevice, std::span<const char*> extensions);

		std::uint32_t GetQueueFamilyIndex(Queue::Type queueType);

		std::uint32_t GetQueueFamilyIndex(std::uint32_t queueFlag);

		Queue GetQueue(Queue::Type queueType);

		[[nodiscard]] VkDevice* Get();

		void WaitIdle() const;

	private:
		PhysicalDevice* _physicalDevice;
		VkDevice _device;
	};

} // Concerto::Graphics::Wrapper

#endif //CONCERTOGRAPHICS_DEVICE_HPP
