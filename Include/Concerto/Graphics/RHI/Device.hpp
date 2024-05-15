//
// Created by arthur on 12/05/2024.
//

#ifndef CONCERTO_GRAPHICS_RHI_DEVICE_HPP
#define CONCERTO_GRAPHICS_RHI_DEVICE_HPP


namespace Concerto::Graphics::RHI
{
	class Device
	{
	public:
		Device() = default;
		virtual ~Device() = default;
		std::unique_ptr<SwapChain> CreateSwapChain();
	};
}

#endif //CONCERTO_GRAPHICS_RHI_DEVICE_HPP