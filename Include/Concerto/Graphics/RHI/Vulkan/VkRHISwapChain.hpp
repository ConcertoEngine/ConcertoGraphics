//
// Created by arthur on 12/05/2024.
//

#ifndef CONCERTO_GRAPHICS_BACKEND_RHI_VULKAN_SWAPCHAIN_HPP
#define CONCERTO_GRAPHICS_BACKEND_RHI_VULKAN_SWAPCHAIN_HPP

#include "Concerto/Graphics/Defines.hpp"
#include "Concerto/Graphics/Backend/Vulkan/Wrapper/RenderPass.hpp"
#include "Concerto/Graphics/RHI/SwapChain.hpp"
#include "Concerto/Graphics/Backend/Vulkan/Wrapper/Swapchain.hpp"
#include "Concerto/Graphics/RHI/FrameBuffer.hpp"

namespace Concerto::Graphics::RHI
{
	class CONCERTO_GRAPHICS_API VkRHISwapChain final : public RHI::SwapChain, public Vk::Swapchain
	{
	public:
		VkRHISwapChain(Vk::Device& device, Window& window, PixelFormat pixelFormat, PixelFormat depthPixelFormat);
		void Present() override;
		RHI::RenderPass& GetRenderPass() override;
		Vector2u GetExtent() override;
	private:
		void CreateFrameBuffers();
		void CreateRenderPass();

		std::vector<Vk::FrameBuffer> _frameBuffers;
		std::unique_ptr<RHI::RenderPass> _renderPass;
	};
}

#endif //CONCERTO_GRAPHICS_BACKEND_RHI_VULKAN_SWAPCHAIN_HPP