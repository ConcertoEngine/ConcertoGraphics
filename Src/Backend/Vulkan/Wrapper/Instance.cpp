//
// Created by arthur on 25/10/2022.
//

#include <cassert>
#include <stdexcept>
#include <iostream>
#include <vector>

#include <Concerto/Core/Assert.hpp>

#include "Concerto/Graphics/Backend/Vulkan/Defines.hpp"
#define VOLK_IMPLEMENTATION
#include <volk.h> // must be under this ^ include

#include "Concerto/Graphics/Backend/Vulkan/Wrapper/Instance.hpp"
#include "Concerto/Graphics/Backend/Vulkan/Wrapper/PhysicalDevice.hpp"

namespace Concerto::Graphics::Vk
{
	PFN_vkGetInstanceProcAddr Instance::vkGetInstanceProcAddr = nullptr;

	VKAPI_ATTR VkBool32 VKAPI_CALL DebugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
			[[maybe_unused]] VkDebugUtilsMessageTypeFlagsEXT messageType,
			[[maybe_unused]] const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
			[[maybe_unused]] void* pUserData)
	{
		if (messageSeverity == VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT)
		{
			//Logger::Info(pCallbackData->pMessage);
		}
		else if (messageSeverity == VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT)
		{
			//Logger::Info(pCallbackData->pMessage);
		}
		else if (messageSeverity == VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT)
		{
			//Logger::Warning(pCallbackData->pMessage);
		}
		else if (messageSeverity == VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT)
		{
			//Logger::Warning(pCallbackData->pMessage);
		}
		return VK_FALSE;
	}

	Instance::Instance(const std::string& appName, const std::string& engineName, const Version& apiVersion,
			const Version& appVersion, const Version& engineVersion, std::span<const char*> extensions,
			std::span<const char*> layers) :
		_instance(VK_NULL_HANDLE),
		_apiVersion(apiVersion),
		_lastResult(VK_SUCCESS)
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
		createInfo.enabledExtensionCount = static_cast<UInt32>(extensions.size());
		createInfo.ppEnabledExtensionNames = extensions.empty() ? VK_NULL_HANDLE : extensions.data();
		createInfo.enabledLayerCount = static_cast<UInt32>(layers.size());
		createInfo.ppEnabledLayerNames = layers.empty() ? VK_NULL_HANDLE : layers.data();
		createInfo.pNext = &debugCreateInfo;

		for (auto& ext : extensions)
			_loadedExtensions.emplace(ext);
		for (auto& layer : layers)
			_loadedLayers.emplace(layer);

		_lastResult = volkInitialize();
		if (_lastResult != VK_SUCCESS)
		{
			CONCERTO_ASSERT_FALSE("ConcertoGraphics: volkInitialize() failed VkResult={}", static_cast<int>(_lastResult));
			throw std::runtime_error("volkInitialize Failed");
		}

		_lastResult = vkCreateInstance(&createInfo, nullptr, &_instance);
		CONCERTO_ASSERT(_lastResult == VK_SUCCESS, "ConcertoGraphics: vkCreateInstance failed VKResult={}", static_cast<int>(_lastResult));
		volkLoadInstanceOnly(_instance);
		Instance::vkGetInstanceProcAddr = ::vkGetInstanceProcAddr;
		#define CONCERTO_VULKAN_BACKEND_INSTANCE_FUNCTION(func) this->func = ::func;

		#define CONCERTO_VULKAN_BACKEND_INSTANCE_EXT_BEGIN(ext)				\
					if(IsExtensionEnabled(#ext))							\
					{
		#define CONCERTO_VULKAN_BACKEND_INSTANCE_EXT_FUNCTION(func, ...)	\
						CONCERTO_VULKAN_BACKEND_INSTANCE_FUNCTION(func)		\
						if (this->func == nullptr)							\
						{													\
							CONCERTO_ASSERT_FALSE("ConcertoGraphics: Function: " #func " is null but the extension has been reported has supported");\
						}
		#define CONCERTO_VULKAN_BACKEND_INSTANCE_EXT_END }

		#include "Concerto/Graphics/Backend/Vulkan/Wrapper/InstanceFunction.hpp"
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
		UInt32 deviceCount = 0;
		_lastResult = vkEnumeratePhysicalDevices(_instance, &deviceCount, nullptr);
		CONCERTO_ASSERT(_lastResult == VK_SUCCESS, "ConcertoGraphics: vkEnumeratePhysicalDevices failed VKResult={}", static_cast<int>(_lastResult));
		std::vector<VkPhysicalDevice> devices(deviceCount);
		_lastResult = vkEnumeratePhysicalDevices(_instance, &deviceCount, devices.data());
		CONCERTO_ASSERT(_lastResult == VK_SUCCESS, "ConcertoGraphics: vkEnumeratePhysicalDevices failed VKResult={}", static_cast<int>(_lastResult));
		std::vector<PhysicalDevice> physicalDevices;
		for(VkPhysicalDevice device : devices)
		{
			physicalDevices.emplace_back(const_cast<Instance&>(*this), device);
		}
		_physicalDevices = std::move(physicalDevices);
		return _physicalDevices.value();
	}

	VkResult Instance::GetLastError() const
	{
		return _lastResult;
	}

	bool Instance::IsExtensionEnabled(const std::string& ext) const
	{
		return _loadedExtensions.contains(ext);
	}
} // namespace Concerto::Graphics::Vk
