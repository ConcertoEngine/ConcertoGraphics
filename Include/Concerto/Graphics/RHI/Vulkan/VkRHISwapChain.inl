//
// Created by arthur on 12/05/2024.
//

#ifndef CONCERTO_GRAPHICS_BACKEND_RHI_VULKAN_SWAPCHAIN_INL
#define CONCERTO_GRAPHICS_BACKEND_RHI_VULKAN_SWAPCHAIN_INL

#include "Concerto/Graphics/RHI/Vulkan/VkRHISwapChain.hpp"

namespace Concerto::Graphics::RHI
{
	inline SwapChain::SwapChain(PixelFormat pixelFormat, PixelFormat depthPixelFormat) :
		_pixelFormat(pixelFormat),
		_depthPixelFormat(depthPixelFormat)
	{
	}

	inline PixelFormat SwapChain::GetPixelFormat() const
	{
		return _pixelFormat;
	}

	inline PixelFormat SwapChain::GetDepthPixelFormat() const
	{
		return _depthPixelFormat;
	}
}

#endif //CONCERTO_GRAPHICS_BACKEND_RHI_VULKAN_SWAPCHAIN_INL