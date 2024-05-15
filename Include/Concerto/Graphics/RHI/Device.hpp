//
// Created by arthur on 12/05/2024.
//

#ifndef CONCERTO_GRAPHICS_RHI_DEVICE_HPP
#define CONCERTO_GRAPHICS_RHI_DEVICE_HPP

#include <string>
#include <memory>

#include "Concerto/Graphics/Defines.hpp"
#include "Concerto/Graphics/RHI/Enums.hpp"

namespace Concerto::Graphics
{
	class Window;
}

namespace Concerto::Graphics::RHI
{
	class SwapChain;

	struct DeviceInfo
	{
		std::string name;
		UInt32 vendor;
		DeviceType type;
	};

	class CONCERTO_GRAPHICS_API Device
	{
	public:
		Device() = default;
		virtual ~Device() = default;
		virtual std::unique_ptr<SwapChain> CreateSwapChain(Window& window) = 0;
	};
}

#endif //CONCERTO_GRAPHICS_RHI_DEVICE_HPP