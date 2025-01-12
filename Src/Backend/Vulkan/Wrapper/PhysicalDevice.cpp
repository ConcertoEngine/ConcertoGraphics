//
// Created by arthur on 25/10/2022.
//

#include <stdexcept>
#include <vector>
#include <utility>

#include <Concerto/Core/Assert.hpp>

#include "Concerto/Graphics/Backend/Vulkan/Wrapper/Instance.hpp"
#include "Concerto/Graphics/Backend/Vulkan/Wrapper/PhysicalDevice.hpp"

namespace cct::gfx::vk
{
	PhysicalDevice::PhysicalDevice(vk::Instance& instance, VkPhysicalDevice physicalDevice) :
		_physicalDevice(physicalDevice),
		_instance(&instance)
	{

	}

	std::span<VkQueueFamilyProperties> PhysicalDevice::GetQueueFamilyProperties() const
	{
		if (_queueFamilyProperties)
			return _queueFamilyProperties.value();
		UInt32 queueFamilyCount = 0;
		_instance->vkGetPhysicalDeviceQueueFamilyProperties(_physicalDevice, &queueFamilyCount, nullptr);
		std::vector<VkQueueFamilyProperties> queueFamilyProperties(queueFamilyCount);
		_instance->vkGetPhysicalDeviceQueueFamilyProperties(_physicalDevice, &queueFamilyCount, queueFamilyProperties.data());
		_queueFamilyProperties = std::move(queueFamilyProperties);
		return _queueFamilyProperties.value();
	}

	PhysicalDevice::PhysicalDevice(PhysicalDevice&& other) noexcept
	{
		_queueFamilyProperties = std::exchange(other._queueFamilyProperties, std::nullopt);
		_physicalDeviceProperties = std::exchange(other._physicalDeviceProperties, std::nullopt);
		_physicalDeviceFeatures = std::exchange(other._physicalDeviceFeatures, std::nullopt);
		_physicalDeviceMemoryProperties = std::exchange(other._physicalDeviceMemoryProperties, std::nullopt);
		_extensionProperties = std::exchange(other._extensionProperties, std::nullopt);
		_extensionPropertiesNames = std::exchange(other._extensionPropertiesNames, std::nullopt);
		_capabilities = std::exchange(other._capabilities, std::nullopt);
		_formats = std::exchange(other._formats, std::nullopt);
		_presentModes = std::exchange(other._presentModes, std::nullopt);
		_physicalDevice = std::exchange(other._physicalDevice, nullptr);
		_instance = other._instance;
	}

	PhysicalDevice& PhysicalDevice::operator=(PhysicalDevice&& other) noexcept
	{
		_queueFamilyProperties = std::exchange(other._queueFamilyProperties, std::nullopt);
		_physicalDeviceProperties = std::exchange(other._physicalDeviceProperties, std::nullopt);
		_physicalDeviceFeatures = std::exchange(other._physicalDeviceFeatures, std::nullopt);
		_physicalDeviceMemoryProperties = std::exchange(other._physicalDeviceMemoryProperties, std::nullopt);
		_extensionProperties = std::exchange(other._extensionProperties, std::nullopt);
		_extensionPropertiesNames = std::exchange(other._extensionPropertiesNames, std::nullopt);
		_capabilities = std::exchange(other._capabilities, std::nullopt);
		_formats = std::exchange(other._formats, std::nullopt);
		_presentModes = std::exchange(other._presentModes, std::nullopt);
		_physicalDevice = std::exchange(other._physicalDevice, nullptr);
		_instance = other._instance;
		return *this;
	}

	VkPhysicalDeviceProperties PhysicalDevice::GetProperties() const
	{
		if (_physicalDeviceProperties)
			return _physicalDeviceProperties.value();
		VkPhysicalDeviceProperties properties;
		_instance->vkGetPhysicalDeviceProperties(_physicalDevice, &properties);
		_physicalDeviceProperties = properties;
		return _physicalDeviceProperties.value();
	}

	VkPhysicalDeviceFeatures PhysicalDevice::GetFeatures() const
	{
		if (_physicalDeviceFeatures)
			return _physicalDeviceFeatures.value();
		VkPhysicalDeviceFeatures features;
		_instance->vkGetPhysicalDeviceFeatures(_physicalDevice, &features);
		_physicalDeviceFeatures = features;
		return _physicalDeviceFeatures.value();
	}

	VkPhysicalDeviceMemoryProperties PhysicalDevice::GetMemoryProperties() const
	{
		if (_physicalDeviceMemoryProperties)
			return _physicalDeviceMemoryProperties.value();
		VkPhysicalDeviceMemoryProperties memoryProperties;
		_instance->vkGetPhysicalDeviceMemoryProperties(_physicalDevice, &memoryProperties);
		_physicalDeviceMemoryProperties = memoryProperties;
		return _physicalDeviceMemoryProperties.value();
	}

	std::span<VkExtensionProperties> PhysicalDevice::GetExtensionProperties() const
	{
		if (_extensionProperties)
			return _extensionProperties.value();
		UInt32 extensionCount = 0;
		_instance->vkEnumerateDeviceExtensionProperties(_physicalDevice, nullptr, &extensionCount, nullptr);
		std::vector<VkExtensionProperties> extensions(extensionCount);
		_instance->vkEnumerateDeviceExtensionProperties(_physicalDevice, nullptr, &extensionCount, extensions.data());
		_extensionProperties = extensions;
		return _extensionProperties.value();
	}

	VkPhysicalDevice* PhysicalDevice::Get()
	{
		CCT_ASSERT(_physicalDevice != VK_NULL_HANDLE, "ConcertoGraphics: physical device handle is null");
		return &_physicalDevice;
	}

	VkPhysicalDevice PhysicalDevice::Get() const
	{
		return _physicalDevice;
	}

	std::span<const char*> PhysicalDevice::GetExtensionPropertiesNames() const
	{
		if (_extensionPropertiesNames)
			return _extensionPropertiesNames.value();
		std::span<VkExtensionProperties> extensions = GetExtensionProperties();
		std::vector<const char*> extensionsNames;
		extensionsNames.reserve(extensions.size());
		for (const auto& extension : extensions)
			extensionsNames.push_back(extension.extensionName);
		_extensionPropertiesNames = extensionsNames;
		return _extensionPropertiesNames.value();
	}

	VkSurfaceCapabilitiesKHR PhysicalDevice::GetCapabilities(VkSurfaceKHR surface) const
	{
		if (_capabilities)
			return _capabilities.value();
		VkSurfaceCapabilitiesKHR capabilities;
		const VkResult result = _instance->vkGetPhysicalDeviceSurfaceCapabilitiesKHR(_physicalDevice, surface, &capabilities);
		CCT_ASSERT(result == VK_SUCCESS, "ConcertoGraphics: vkGetPhysicalDeviceSurfaceCapabilitiesKHR failed VKResult={}", static_cast<int>(result));
		_capabilities = capabilities;
		return _capabilities.value();
	}

	std::span<VkSurfaceFormatKHR> PhysicalDevice::GetFormats(VkSurfaceKHR surface) const
	{
		if (_formats)
			return _formats.value();
		UInt32 formatCount = 0;
		_instance->vkGetPhysicalDeviceSurfaceFormatsKHR(_physicalDevice, surface, &formatCount, nullptr);
		std::vector<VkSurfaceFormatKHR> formats(formatCount);
		_instance->vkGetPhysicalDeviceSurfaceFormatsKHR(_physicalDevice, surface, &formatCount, formats.data());
		_formats = formats;
		return _formats.value();
	}

	std::span<VkPresentModeKHR> PhysicalDevice::GetPresentModes(VkSurfaceKHR surface) const
	{
		if (_presentModes)
			return _presentModes.value();
		UInt32 count = 0;
		_instance->vkGetPhysicalDeviceSurfacePresentModesKHR(_physicalDevice, surface, &count, nullptr);
		std::vector<VkPresentModeKHR> modes(count);
		_instance->vkGetPhysicalDeviceSurfacePresentModesKHR(_physicalDevice, surface, &count, modes.data());
		_presentModes = modes;
		return _presentModes.value();
	}

	PhysicalDevice::SurfaceSupportDetails PhysicalDevice::GetSurfaceSupportDetails(VkSurfaceKHR surface) const
	{
		SurfaceSupportDetails details;
		details.capabilities = GetCapabilities(surface);
		details.formats = GetFormats(surface);
		details.presentModes = GetPresentModes(surface);
		return details;
	}
} // cct::gfx::vk