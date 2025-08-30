//
// Created by arthur on 12/05/2024.
//

#ifndef CONCERTO_GRAPHICS_BACKEND_RHI_VULKAN_SWAPCHAIN_INL
#define CONCERTO_GRAPHICS_BACKEND_RHI_VULKAN_SWAPCHAIN_INL

#include "Concerto/Graphics/RHI/Vulkan/VkRHISwapChain/VkRHISwapChain.hpp"

namespace cct::gfx::rhi
{
	inline SwapChain::SwapChain(PixelFormat pixelFormat, PixelFormat depthPixelFormat) :
		m_pixelFormat(pixelFormat),
		m_depthPixelFormat(depthPixelFormat)
	{
	}

	inline PixelFormat SwapChain::GetPixelFormat() const
	{
		return m_pixelFormat;
	}

	inline PixelFormat SwapChain::GetDepthPixelFormat() const
	{
		return m_depthPixelFormat;
	}
}

#endif //CONCERTO_GRAPHICS_BACKEND_RHI_VULKAN_SWAPCHAIN_INL