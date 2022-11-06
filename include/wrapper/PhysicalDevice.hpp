//
// Created by arthur on 25/10/2022.
//

#ifndef CONCERTOGRAPHICS_PHYSICALDEVICE_HPP
#define CONCERTOGRAPHICS_PHYSICALDEVICE_HPP

#include <span>
#include <optional>
#include <vector>
#include "vulkan/vulkan.h"

namespace Concerto::Graphics::Wrapper
{
	class Instance;

	class PhysicalDevice
	{
	public:
		struct SurfaceSupportDetails
		{
			VkSurfaceCapabilitiesKHR capabilities{};
			std::span<VkSurfaceFormatKHR> formats{};
			std::span<VkPresentModeKHR> presentModes{};
		};

		PhysicalDevice() = default;

		explicit PhysicalDevice(VkPhysicalDevice physicalDevice);
		PhysicalDevice(const PhysicalDevice&) = delete;
		PhysicalDevice(PhysicalDevice&&) noexcept;
		PhysicalDevice& operator=(const PhysicalDevice&) = delete;
		PhysicalDevice& operator=(PhysicalDevice&&) noexcept;

		[[nodiscard]] std::span<VkQueueFamilyProperties> GetQueueFamilyProperties() const;

		[[nodiscard]] VkPhysicalDeviceProperties GetProperties() const;

		[[nodiscard]] VkPhysicalDeviceFeatures GetFeatures() const;

		[[nodiscard]] VkPhysicalDeviceMemoryProperties GetMemoryProperties() const;

		[[nodiscard]] std::span<VkExtensionProperties> GetExtensionProperties() const;

		[[nodiscard]] std::span<const char*> GetExtensionPropertiesNames() const;

		VkSurfaceCapabilitiesKHR GetCapabilities() const;

		std::span<VkSurfaceFormatKHR> GetFormats() const;

		std::span<VkPresentModeKHR> GetPresentModes() const;

		SurfaceSupportDetails GetSurfaceSupportDetails() const;

		[[nodiscard]] VkPhysicalDevice* Get();

		[[nodiscard]] VkSurfaceKHR GetSurface();

		void SetSurface(VkSurfaceKHR surface);

	private:
		mutable std::optional<std::vector<VkQueueFamilyProperties>> _queueFamilyProperties;
		mutable std::optional<VkPhysicalDeviceProperties> _physicalDeviceProperties;
		mutable std::optional<VkPhysicalDeviceFeatures> _physicalDeviceFeatures;
		mutable std::optional<VkPhysicalDeviceMemoryProperties> _physicalDeviceMemoryProperties;
		mutable std::optional<std::vector<VkExtensionProperties>> _extensionProperties;
		mutable std::optional<std::vector<const char*>> _extensionPropertiesNames;
		mutable std::optional<VkSurfaceCapabilitiesKHR> _capabilities;
		mutable std::optional<std::vector<VkSurfaceFormatKHR>> _formats;
		mutable std::optional<std::vector<VkPresentModeKHR>> _presentModes;
		VkPhysicalDevice _physicalDevice{ VK_NULL_HANDLE };
		VkSurfaceKHR _surface{ VK_NULL_HANDLE };
	};

} // Concerto::Graphics::Wrapper

#endif //CONCERTOGRAPHICS_PHYSICALDEVICE_HPP
