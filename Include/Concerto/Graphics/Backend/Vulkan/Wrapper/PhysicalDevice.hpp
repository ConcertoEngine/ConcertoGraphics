//
// Created by arthur on 25/10/2022.
//

#ifndef CONCERTO_GRAPHICS_PHYSICALDEVICE_HPP
#define CONCERTO_GRAPHICS_PHYSICALDEVICE_HPP

#include <span>
#include <optional>
#include <vector>

#include <vulkan/vulkan.h>

#include "Concerto/Graphics/Backend/Vulkan/Defines.hpp"

namespace Concerto::Graphics::Vk
{
	class Instance;

	/**
	* @class PhysicalDevice
	*
	* @brief Represents a physical device in a Vulkan instance.
	*
	* The PhysicalDevice class represents a physical device, such as a GPU, in a Vulkan instance.
	* It is responsible for providing information about the device and its capabilities, such as its queue families, properties, features, and memory properties.
	*/
	class CONCERTO_GRAPHICS_VULKAN_BACKEND_API PhysicalDevice
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

		~PhysicalDevice() = default;

		PhysicalDevice(const PhysicalDevice&) = delete;
		
		PhysicalDevice(PhysicalDevice&&) noexcept;
		
		PhysicalDevice& operator=(const PhysicalDevice&) = delete;
		
		PhysicalDevice& operator=(PhysicalDevice&&) noexcept;

		/**
		* @brief Get the queue family properties of the physical device.
		*
		* This function is used to get the properties of the queue families of the physical device.
		*
		* @return The queue family properties of the physical device.
		*/
		[[nodiscard]] std::span<VkQueueFamilyProperties> GetQueueFamilyProperties() const;

		/**
		* @brief Get the properties of the physical device.
		*
		* @return The properties of the physical device.
		*/
		[[nodiscard]] VkPhysicalDeviceProperties GetProperties() const;

		/**
		* @brief Get the features of the physical device.
		*
		* @return The features of the physical device.
		*/
		[[nodiscard]] VkPhysicalDeviceFeatures GetFeatures() const;

		/**
		* @brief Get the memory properties of the physical device.
		*
		* @return The memory properties of the physical device.
		*/
		[[nodiscard]] VkPhysicalDeviceMemoryProperties GetMemoryProperties() const;

		/**
		* @brief Get the extension properties of the physical device.
		*
		* @return The extension properties of the physical device.
		*/
		[[nodiscard]] std::span<VkExtensionProperties> GetExtensionProperties() const;

		/**
		 * @brief Get the extension properties names of the physical device.
		 *
		 * @return The extension properties names of the physical device.
		*/
		[[nodiscard]] std::span<const char*> GetExtensionPropertiesNames() const;

		VkSurfaceCapabilitiesKHR GetCapabilities() const;

		std::span<VkSurfaceFormatKHR> GetFormats() const;

		std::span<VkPresentModeKHR> GetPresentModes() const;

		SurfaceSupportDetails GetSurfaceSupportDetails() const;

		[[nodiscard]] VkPhysicalDevice* Get();

		[[nodiscard]] VkPhysicalDevice Get() const;

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

} // Concerto::Graphics::Vk

#endif //CONCERTO_GRAPHICS_PHYSICALDEVICE_HPP
