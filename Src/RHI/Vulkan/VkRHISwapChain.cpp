//
// Created by arthur on 15/05/2024.
//

#include "Concerto/Graphics/RHI/Vulkan/VkRHISwapChain.hpp"
#include "Concerto/Graphics/RHI/Vulkan/Utils.hpp"

namespace Concerto::Graphics::RHI
{
	VkRHISwapChain::VkRHISwapChain(Vk::Device& device, Window& window, PixelFormat pixelFormat, PixelFormat depthPixelFormat) :
		SwapChain(pixelFormat, depthPixelFormat),
		Vk::Swapchain(device, window, Converters::ToVulkan(pixelFormat), Converters::ToVulkan(depthPixelFormat))
	{
	}

	void VkRHISwapChain::Present()
	{
		CONCERTO_ASSERT_FALSE("Not Implemented");
	}
}
