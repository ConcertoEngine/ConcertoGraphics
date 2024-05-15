//
// Created by arthur on 12/05/2024.
//

#ifndef CONCERTO_GRAPHICS_RHI_INSTANCE_HPP
#define CONCERTO_GRAPHICS_RHI_INSTANCE_HPP

#include <span>

#include "Concerto/Graphics/RHI/RHIDevice.hpp"

namespace Concerto::Graphics::RHI
{
	class Device;

	enum class DeviceType
	{
		Other,      //Unknown device type
		Integrated, //GPU integrated to the CPU
		Dedicated,  //GPU dedicated to the graphics
		Virtual,    //Virtual GPU provided by a virtualization system
		Software    //CPU software renderer
	};

	struct DeviceInfo
	{
		std::string name;
		std::string vendor;
		DeviceType type;
	};

	class Instance
	{
	public:
		enum class Backend
		{
			ConcertoVulkan, // ConcertoGraphics Vulkan backend
			Vulkan, // WebGPU Vulkan backend
			DirectX11, // WebGPU DirectX11 backend
			DirectX12, // WebGPU DirectX12 backend
			Metal, // WebGPU Metal backend
			OpenGL, // WebGPU OpenGL backend
			OpenGLES, // WebGPU OpenGLES backend
		};

		enum class ValidationLevel
		{
			None = 0,
			Info = 1,
			Warning = 2,
			Error = 3,
			All = 4
		};

		Instance(Backend backend = Backend::ConcertoVulkan) {}

		const std::span<DeviceInfo>& EnumarateDevices() const {}
		std::unique_ptr<Device> CreateDevice(std::size_t index) {}
	};
}

#endif //CONCERTO_GRAPHICS_RHI_INSTANCE_HPP