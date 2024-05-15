//
// Created by arthur on 12/05/2024.
//

#ifndef CONCERTO_GRAPHICS_BACKEND_RHI_VULKAN_SWAPCHAIN_HPP
#define CONCERTO_GRAPHICS_BACKEND_RHI_VULKAN_SWAPCHAIN_HPP

#include "Concerto/Graphics/Defines.hpp"
#include "Concerto/Graphics/RHI/SwapChain.hpp"
#include "Concerto/Graphics/Backend/Vulkan/Wrapper/Swapchain.hpp"

namespace Concerto::Graphics::RHI
{
	class CONCERTO_GRAPHICS_API VkRHISwapChain final : public SwapChain, public Vk::Swapchain
	{
	public:
		VkRHISwapChain(Vk::Device& device, Window& window);
		void Present() override;
	};
}

#endif //CONCERTO_GRAPHICS_BACKEND_RHI_VULKAN_SWAPCHAIN_HPP