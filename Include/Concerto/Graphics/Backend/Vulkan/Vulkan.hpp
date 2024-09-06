//
// Created by arthur on 17/08/2023.
//

#ifndef CONCERTO_GRAPHICS_INCLUDE_VULKAN_HPP_
#define CONCERTO_GRAPHICS_INCLUDE_VULKAN_HPP_

#include <span>
#include <string>
#include <map>

#include "Concerto/Graphics/Backend/Vulkan/Defines.hpp"
#include "Concerto/Graphics/RHI/Enums.hpp"
#include "Concerto/Graphics/Version.hpp"
#include "Concerto/Graphics/Backend/Vulkan/Wrapper/Instance.hpp"

namespace Concerto::Graphics::Vk
{
	class Device;
	class PhysicalDevice;

	struct RendererInfo
	{
		std::string applicationName;
		Version applicationVersion;
		bool useImGUI = false;
		UInt32 width;
		UInt32 height;
	};

	class CONCERTO_GRAPHICS_VULKAN_BACKEND_API Vulkan
	{
	public:
		Vulkan(RendererInfo info);
		~Vulkan() = default;

		static Vulkan* GetInstance();

		Device* CreateDevice(RHI::DeviceType type = RHI::DeviceType::Dedicated);

		Instance& GetVkInstance();

	private:
		static Vulkan* _instance;
		RendererInfo _info;
		Instance _vkInstance;
		std::span<PhysicalDevice> _physicalDevices;
		std::multimap<RHI::DeviceType, Device> _devices;
	};
}

#endif //CONCERTO_GRAPHICS_INCLUDE_VULKAN_HPP_