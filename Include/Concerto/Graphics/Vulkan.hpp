//
// Created by arthur on 17/08/2023.
//

#ifndef CONCERTOGRAPHICS_INCLUDE_VULKAN_HPP_
#define CONCERTOGRAPHICS_INCLUDE_VULKAN_HPP_

#include <span>
#include <string>
#include <map>

#include "Concerto/Graphics/Defines.hpp"

#include "Concerto/Graphics/Version.hpp"
#include "Concerto/Graphics/Vulkan/Wrapper/Instance.hpp"

namespace Concerto::Graphics
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

	enum class DeviceType
	{
		Other, //Unknown device type
		Integrated, //GPU integrated to the CPU
		Dedicated, //GPU dedicated to the graphics
		Virtual, //Virtual GPU provided by a virtualization system
		Software //CPU software renderer
	};
	
	class CONCERTO_GRAPHICS_API Vulkan
	{
	public:
		Vulkan(RendererInfo info);
		~Vulkan() = default;
		
		static Vulkan* GetInstance();
		
		Device* CreateDevice(DeviceType type = DeviceType::Dedicated);
		
		Instance& GetVkInstance();

	private:
		static Vulkan* _instance;
		RendererInfo _info;
		Instance _vkInstance;
		std::span<PhysicalDevice> _physicalDevices;
		std::multimap<DeviceType, Device> _devices;
	};
}

#endif //CONCERTOGRAPHICS_INCLUDE_VULKAN_HPP_