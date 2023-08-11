//
// Created by arthur on 25/10/2022.
//

#include <cassert>
#include <stdexcept>
#include <iostream>
#include <vector>

#include "Concerto/Graphics/Vulkan/Wrapper/Instance.hpp"
#include "Concerto/Graphics/Vulkan/Wrapper/PhysicalDevice.hpp"

namespace Concerto::Graphics
{
	VKAPI_ATTR VkBool32 VKAPI_CALL DebugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
			VkDebugUtilsMessageTypeFlagsEXT messageType, const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
			void* pUserData)
	{
		if (messageSeverity == VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT)
		{
//			std::cout << "[Verbose] " << pCallbackData->pMessage << std::endl;
		}
		else if (messageSeverity == VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT)
		{
			std::cout << "[Info] " << pCallbackData->pMessage << std::endl;
		}
		else if (messageSeverity == VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT)
		{
			std::cout << "[Warning] " << pCallbackData->pMessage << std::endl;
		}
		else if (messageSeverity == VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT)
		{
			std::cerr << "[Error] " << pCallbackData->pMessage << std::endl;
		}
		return VK_FALSE;
	}

	Instance::Instance(const std::string& appName, const std::string& engineName, const Version& apiVersion,
			const Version& appVersion, const Version& engineVersion, std::span<const char*> extensions,
			std::span<const char*> layers) : _instance(VK_NULL_HANDLE), _lastResult(VK_SUCCESS), _apiVersion(apiVersion)
	{
		VkApplicationInfo appInfo = {};
		appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
		appInfo.pApplicationName = appName.c_str();
		appInfo.applicationVersion = VK_MAKE_VERSION(appVersion.major, appVersion.minor, appVersion.patch);
		appInfo.pEngineName = engineName.c_str();
		appInfo.engineVersion = VK_MAKE_VERSION(engineVersion.major, engineVersion.minor, engineVersion.patch);
		appInfo.apiVersion = VK_MAKE_VERSION(apiVersion.major, apiVersion.minor, apiVersion.patch);

		VkDebugUtilsMessengerCreateInfoEXT debugCreateInfo = {};
		debugCreateInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
		debugCreateInfo.messageSeverity =
				VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT |
				VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
		debugCreateInfo.messageType =
				VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT |
				VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
		debugCreateInfo.pfnUserCallback = DebugCallback;
		debugCreateInfo.pUserData = nullptr;

		VkInstanceCreateInfo createInfo = {};
		createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
		createInfo.pApplicationInfo = &appInfo;
		createInfo.enabledExtensionCount = extensions.size();
		createInfo.ppEnabledExtensionNames = extensions.empty() ? VK_NULL_HANDLE : extensions.data();
		createInfo.enabledLayerCount = layers.size();
		createInfo.ppEnabledLayerNames = layers.empty() ? VK_NULL_HANDLE : layers.data();
		createInfo.pNext = &debugCreateInfo;

		_lastResult = vkCreateInstance(&createInfo, nullptr, &_instance);
		if (_lastResult != VK_SUCCESS)
		{
			throw std::runtime_error("Failed to create instance" + std::to_string(_lastResult));
		}
	}

	Version Instance::GetApiVersion() const
	{
		return _apiVersion;
	}

	VkInstance* Instance::Get()
	{
		assert(_instance != VK_NULL_HANDLE);
		return &_instance;
	}

	std::span<PhysicalDevice> Instance::EnumeratePhysicalDevices() const
	{
		if (_physicalDevices)
			return _physicalDevices.value();
		std::uint32_t deviceCount = 0;
		_lastResult = vkEnumeratePhysicalDevices(_instance, &deviceCount, nullptr);
		if (_lastResult != VK_SUCCESS)
		{
			throw std::runtime_error("Failed to get physical device count" + std::to_string(_lastResult));
		}
		std::vector<VkPhysicalDevice> devices(deviceCount);
		_lastResult = vkEnumeratePhysicalDevices(_instance, &deviceCount, devices.data());
		if (_lastResult != VK_SUCCESS)
		{
			throw std::runtime_error("Failed to get physical devices" + std::to_string(_lastResult));
		}
		std::vector<PhysicalDevice> physicalDevices;
		for(VkPhysicalDevice device : devices)
		{

			physicalDevices.emplace_back(device);
		}
		_physicalDevices = std::move(physicalDevices);
		return _physicalDevices.value();
	}
} // namespace Concerto::Graphics
