//
// Created by arthur on 12/05/2024.
//

#ifndef CONCERTO_GRAPHICS_RHI_INSTANCE_HPP
#define CONCERTO_GRAPHICS_RHI_INSTANCE_HPP

#include <span>
#include <memory>

#include "Concerto/Graphics/RHI/Defines.hpp"
#include "Concerto/Graphics/RHI/Enums.hpp"
#include "Concerto/Graphics/RHI/Device.hpp"

namespace cct::gfx::rhi
{
	class Device;
	class APIImpl;

	class CONCERTO_GRAPHICS_RHI_BASE_API Instance
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

		Instance(Backend backend = Backend::ConcertoVulkan, ValidationLevel validationLevel = ValidationLevel::All);

		std::span<const DeviceInfo> EnumerateDevices() const;
		std::unique_ptr<Device> CreateDevice(std::size_t index);
		inline APIImpl* GetImpl();
	private:
		std::unique_ptr<APIImpl> _apiImpl;
	};
}

#endif //CONCERTO_GRAPHICS_RHI_INSTANCE_HPP