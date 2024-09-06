//
// Created by arthur on 12/05/2024.
//
#include <vector>

#include <Concerto/Core/Assert.hpp>

#include "Concerto/Graphics/RHI/Vulkan/VkRHI.hpp"
#include "Concerto/Graphics/RHI/Vulkan/VkRHIDevice.hpp"
#ifdef CONCERTO_PLATFORM_WINDOWS
#include <Windows.h>
#include <vulkan/vulkan_win32.h>
#elif defined(CONCERTO_PLATFORM_POSIX)
#error "fixme"
#endif

namespace Concerto::Graphics
{
	bool VkRHI::Create(RHI::ValidationLevel validationLevel)
	{
		std::vector<const char*> extensions = {VK_KHR_SURFACE_EXTENSION_NAME};
#if defined(CONCERTO_PLATFORM_WINDOWS)
		extensions.push_back(VK_KHR_WIN32_SURFACE_EXTENSION_NAME);
#elif defined(CONCERTO_PLATFORM_POSIX)
		#error "fixme"
#endif
		if (validationLevel != RHI::ValidationLevel::None)
		{
			extensions.push_back(VK_EXT_DEBUG_REPORT_EXTENSION_NAME);
		}
#ifdef CONCERTO_DEBUG
		std::vector<const char*> layers = { "VK_LAYER_KHRONOS_validation" /*, "VK_LAYER_LUNARG_api_dump"*/ };
#else
		std::vector<const char*> layers = {};
#endif
		_instance = std::make_unique<Vk::Instance>("", "", Version{ 1, 3, 0 }, Version{}, Version{}, extensions, layers);
		if (_instance->GetLastError() != VK_SUCCESS)
		{
			CONCERTO_ASSERT_FALSE("ConcertoGraphics: Failed to initialize Vulkan instance, VkResult={}", static_cast<Int32>(_instance->GetLastError()));
			return false;
		}
		return true;
	}

	std::span<const RHI::DeviceInfo> VkRHI::EnumerateDevices()
	{
		if (!_instance)
		{
			CONCERTO_ASSERT_FALSE("ConcertoGraphics: Invalid Vulkan instance, please call VkRHI::Create before");
			return _devicesInfo;
		}
		const auto devices = _instance->EnumeratePhysicalDevices();
		for (auto& device : devices)
		{
			_devicesInfo.emplace_back(
				device.GetProperties().deviceName,
				device.GetProperties().vendorID,
				FromVulkan(device.GetProperties().deviceType)
			);
		}
		return _devicesInfo;
	}

	std::unique_ptr<RHI::Device> VkRHI::CreateDevice(std::size_t index)
	{
		CONCERTO_ASSERT(index <= _devicesInfo.size(), "ConcertoGraphics: Device index out of range");
		if (!_instance)
		{
			CONCERTO_ASSERT_FALSE("ConcertoGraphics: Invalid Vulkan instance, please call VkRHI::Create before");
			return nullptr;
		}
		std::unique_ptr<RHI::Device> device;
		const auto physicalDevices = _instance->EnumeratePhysicalDevices();
		for (std::size_t i = 0; i < physicalDevices.size(); ++i) 
		{
			if (i != index)
				continue;
			device = std::make_unique<RHI::VkRHIDevice>(physicalDevices[i], *_instance);
		}
		CONCERTO_ASSERT(device, "ConcertoGraphics: Unable to create Vulkan device");
		return std::move(device);
	}

	constexpr RHI::DeviceType VkRHI::FromVulkan(VkPhysicalDeviceType deviceType)
	{
		switch (deviceType)
		{
		case VK_PHYSICAL_DEVICE_TYPE_OTHER:
			return RHI::DeviceType::Other;
		case VK_PHYSICAL_DEVICE_TYPE_INTEGRATED_GPU:
			return RHI::DeviceType::Integrated;
		case VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU:
			return RHI::DeviceType::Dedicated;
		case VK_PHYSICAL_DEVICE_TYPE_VIRTUAL_GPU:
			return RHI::DeviceType::Virtual;
		case VK_PHYSICAL_DEVICE_TYPE_CPU:
			return RHI::DeviceType::Software;
		default:
			return {};
		}
	}
}
