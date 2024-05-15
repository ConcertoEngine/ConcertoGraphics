//
// Created by arthur on 15/05/2024.
//

#include "Concerto/Graphics/RHI/Vulkan/VkRHISwapChain.hpp"

namespace Concerto::Graphics::RHI
{
	VkRHISwapChain::VkRHISwapChain(Vk::Device& device, Window& window) :
		Vk::Swapchain(device, window)
	{
	}

	void VkRHISwapChain::Present()
	{
		CONCERTO_ASSERT_FALSE("Not Implemented");
	}
}
