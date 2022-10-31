//
// Created by arthur on 25/10/2022.
//

#include <cassert>
#include <stdexcept>
#include <vector>
#include "wrapper/PhysicalDevice.hpp"

namespace Concerto::Graphics::Wrapper
{
	PhysicalDevice::PhysicalDevice(VkPhysicalDevice physicalDevice) : _physicalDevice(physicalDevice)
	{

	}

	std::span<VkQueueFamilyProperties> PhysicalDevice::GetQueueFamilyProperties() const
	{
		if (_queueFamilyProperties)
			return _queueFamilyProperties.value();
		std::uint32_t queueFamilyCount = 0;
		vkGetPhysicalDeviceQueueFamilyProperties(_physicalDevice, &queueFamilyCount, nullptr);
		std::vector<VkQueueFamilyProperties> queueFamilyProperties(queueFamilyCount);
		vkGetPhysicalDeviceQueueFamilyProperties(_physicalDevice, &queueFamilyCount, queueFamilyProperties.data());
		_queueFamilyProperties = std::move(queueFamilyProperties);
		return _queueFamilyProperties.value();
	}

	VkPhysicalDeviceProperties PhysicalDevice::GetProperties() const
	{
		if (_physicalDeviceProperties)
			return _physicalDeviceProperties.value();
		VkPhysicalDeviceProperties properties;
		vkGetPhysicalDeviceProperties(_physicalDevice, &properties);
		_physicalDeviceProperties = properties;
		return _physicalDeviceProperties.value();
	}

	VkPhysicalDeviceFeatures PhysicalDevice::GetFeatures() const
	{
		if (_physicalDeviceFeatures)
			return _physicalDeviceFeatures.value();
		VkPhysicalDeviceFeatures features;
		vkGetPhysicalDeviceFeatures(_physicalDevice, &features);
		_physicalDeviceFeatures = features;
		return _physicalDeviceFeatures.value();
	}

	VkPhysicalDeviceMemoryProperties PhysicalDevice::GetMemoryProperties() const
	{
		if (_physicalDeviceMemoryProperties)
			return _physicalDeviceMemoryProperties.value();
		VkPhysicalDeviceMemoryProperties memoryProperties;
		vkGetPhysicalDeviceMemoryProperties(_physicalDevice, &memoryProperties);
		_physicalDeviceMemoryProperties = memoryProperties;
		return _physicalDeviceMemoryProperties.value();
	}

	std::span<VkExtensionProperties> PhysicalDevice::GetExtensionProperties() const
	{
		if (_extensionProperties)
			return _extensionProperties.value();
		std::uint32_t extensionCount = 0;
		vkEnumerateDeviceExtensionProperties(_physicalDevice, nullptr, &extensionCount, nullptr);
		std::vector<VkExtensionProperties> extensions(extensionCount);
		vkEnumerateDeviceExtensionProperties(_physicalDevice, nullptr, &extensionCount, extensions.data());
		_extensionProperties = extensions;
		return _extensionProperties.value();
	}

	VkPhysicalDevice* PhysicalDevice::Get()
	{
		assert(_physicalDevice != VK_NULL_HANDLE);
		return &_physicalDevice;
	}

	std::span<const char*> PhysicalDevice::GetExtensionPropertiesNames() const
	{
		if (_extensionPropertiesNames)
			return _extensionPropertiesNames.value();
		std::span<VkExtensionProperties> extensions = GetExtensionProperties();
		std::vector<const char*> extensionsNames;
		extensionsNames.reserve(extensions.size());
		for (const auto& extension: extensions)
			extensionsNames.push_back(extension.extensionName);
		_extensionPropertiesNames = extensionsNames;
		return _extensionPropertiesNames.value();
	}

	VkSurfaceCapabilitiesKHR PhysicalDevice::GetCapabilities() const
	{
		if (_capabilities)
			return _capabilities.value();
		VkSurfaceCapabilitiesKHR capabilities;
		if (vkGetPhysicalDeviceSurfaceCapabilitiesKHR(_physicalDevice, _surface, &capabilities) != VK_SUCCESS)
		{
			throw std::runtime_error("vkGetPhysicalDeviceSurfaceCapabilitiesKHR fail");
		}
		_capabilities = capabilities;
		return _capabilities.value();
	}

	std::span<VkSurfaceFormatKHR> PhysicalDevice::GetFormats() const
	{
		if (_formats)
			return _formats.value();
		assert(_surface != VK_NULL_HANDLE);
		std::uint32_t formatCount = 0;
		vkGetPhysicalDeviceSurfaceFormatsKHR(_physicalDevice, _surface, &formatCount, nullptr);
		std::vector<VkSurfaceFormatKHR> formats(formatCount);
		vkGetPhysicalDeviceSurfaceFormatsKHR(_physicalDevice, _surface, &formatCount, formats.data());
		_formats = formats;
		return _formats.value();
	}

	std::span<VkPresentModeKHR> PhysicalDevice::GetPresentModes() const
	{
		if (_presentModes)
			return _presentModes.value();
		std::uint32_t count = 0;
		vkGetPhysicalDeviceSurfacePresentModesKHR(_physicalDevice, _surface, &count, nullptr);
		std::vector<VkPresentModeKHR> modes(count);
		vkGetPhysicalDeviceSurfacePresentModesKHR(_physicalDevice, _surface, &count, modes.data());
		_presentModes = modes;
		return _presentModes.value();
	}

	PhysicalDevice::SurfaceSupportDetails PhysicalDevice::GetSurfaceSupportDetails() const
	{
		SurfaceSupportDetails details;
		details.capabilities = GetCapabilities();
		details.formats = GetFormats();
		details.presentModes = GetPresentModes();
		return details;
	}

	VkSurfaceKHR PhysicalDevice::GetSurface()
	{
		return _surface;
	}

	void PhysicalDevice::SetSurface(VkSurfaceKHR surface)
	{
		_surface = surface;
	}
} // Concerto::Graphics::Wrapper