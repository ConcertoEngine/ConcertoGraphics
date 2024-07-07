//
// Created by arthur on 12/05/2024.
//

#ifndef CONCERTO_GRAPHICS_BACKEND_RHI_VULKAN_DEVICE_HPP
#define CONCERTO_GRAPHICS_BACKEND_RHI_VULKAN_DEVICE_HPP

#include "Concerto/Graphics/Defines.hpp"
#include "Concerto/Graphics/RHI/Device.hpp"
#include "Concerto/Graphics/Backend/Vulkan/Wrapper/Device.hpp"

namespace Concerto::Graphics::RHI
{
	class CONCERTO_GRAPHICS_API VkRHIDevice final : public Device, public Vk::Device
	{
	public:
		VkRHIDevice(Vk::PhysicalDevice& physicalDevice, Vk::Instance& instance);
		std::unique_ptr<SwapChain> CreateSwapChain(Window& window, PixelFormat pixelFormat = PixelFormat::BGRA8_SRGB, PixelFormat depthPixelFormat = PixelFormat::D32f) override;
		std::unique_ptr<RenderPass> CreateRenderPass(std::span<RenderPass::Attachment> attachments, std::span<RenderPass::SubPassDescription> subPassDescriptions, std::span<RHI::RenderPass::SubPassDependency> subPassDependencies) override;
		std::unique_ptr<FrameBuffer> CreateFrameBuffer(UInt32 width, UInt32 height, const RenderPass& renderPass, const std::vector<Texture>& attachments) override;

	private:
		VkSurfaceKHR _surface;
		Vk::Instance* _vkInstance;
	};
}

#endif //CONCERTO_GRAPHICS_BACKEND_RHI_VULKAN_DEVICE_HPP