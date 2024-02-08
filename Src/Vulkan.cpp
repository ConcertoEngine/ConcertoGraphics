//
// Created by arthur on 17/08/2023.
//

#include <array>

#include <Concerto/Core/Assert.hpp>

#include "Concerto/Graphics/Vulkan.hpp"
#include "Concerto/Graphics/Vulkan/Wrapper/PhysicalDevice.hpp"
#include "Concerto/Graphics/Vulkan/Wrapper/Device.hpp"

namespace Concerto::Graphics
{
    namespace
    {

#if defined(CONCERTO_PLATFORM_WINDOWS)
        std::array extensions = { "VK_KHR_surface", "VK_KHR_win32_surface", "VK_EXT_debug_report"};
#elif defined(CONCERTO_PLATFORM_POSIX)
        std::array extensions = { "VK_KHR_surface", "VK_KHR_xcb_surface", "VK_KHR_xlib_surface", "VK_KHR_wayland_surface" };
#endif
        std::array layers = { "VK_LAYER_KHRONOS_validation" /*, "VK_LAYER_LUNARG_api_dump"*/ };
    }

    
    Vulkan* Vulkan::_instance = nullptr;

    Vulkan::Vulkan(RendererInfo info) :
        _info(std::move(info)),
		_vkInstance(_info.applicationName,
                    _info.applicationName,
                    { 1, 3, 0 },
                    _info.applicationVersion,
                    { 1, 0, 0 }, extensions, layers),
        _physicalDevices(_vkInstance.EnumeratePhysicalDevices())
    {
		CONCERTO_ASSERT(_instance == nullptr); // Only one renderer can be created
		_instance = this;
    }

    Vulkan* Concerto::Graphics::Vulkan::GetInstance()
    {
        CONCERTO_ASSERT(_instance != nullptr); // Vulkan must be created before calling Instance()
        return _instance;
    }

    Device* Vulkan::CreateDevice(DeviceType type)
    {
		auto it = _devices.find(type);
        if (it != _devices.end())
            return &it->second;
        for (auto& physicalDevice : _physicalDevices)
        {
            auto properties = physicalDevice.GetProperties();
            DeviceType deviceType = static_cast<DeviceType>(properties.deviceType);
            if (deviceType != type)
                continue;
            it =_devices.emplace(deviceType, Device(physicalDevice, _vkInstance));
            return &it->second;
        }
		return nullptr;
    }

    Instance& Vulkan::GetVkInstance()
    {
        return _vkInstance;
    }
}