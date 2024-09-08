//
// Created by arthur on 12/05/2024.
//

#ifndef CONCERTO_GRAPHICS_BACKEND_RHI_VULKAN_DEVICE_HPP
#define CONCERTO_GRAPHICS_BACKEND_RHI_VULKAN_DEVICE_HPP

#include <optional>

#include "Concerto/Graphics/RHI/Defines.hpp"
#include "Concerto/Graphics/RHI/Device.hpp"
#include "Concerto/Graphics/Backend/Vulkan/Wrapper/Device.hpp"
#include "Concerto/Graphics/Backend/Vulkan/UploadContext.hpp"

namespace Concerto::Graphics::RHI
{
	class CONCERTO_GRAPHICS_RHI_BASE_API VkRHIDevice final : public RHI::Device, public Vk::Device
	{
	public:
		VkRHIDevice(Vk::PhysicalDevice& physicalDevice, Vk::Instance& instance);

		// overrides
		std::unique_ptr<SwapChain> CreateSwapChain(Window& window, PixelFormat pixelFormat = PixelFormat::BGRA8_SRGB, PixelFormat depthPixelFormat = PixelFormat::D32f) override;
		std::unique_ptr<RenderPass> CreateRenderPass(std::span<RenderPass::Attachment> attachments, std::span<RenderPass::SubPassDescription> subPassDescriptions, std::span<RHI::RenderPass::SubPassDependency> subPassDependencies) override;
		std::unique_ptr<FrameBuffer> CreateFrameBuffer(UInt32 width, UInt32 height, const RHI::RenderPass& renderPass, const std::vector<RHI::Texture>& attachments) override;
		std::unique_ptr<MaterialBuilder> CreateMaterialBuilder(const Vector2u& windowExtent) override;
		std::unique_ptr<TextureBuilder> CreateTextureBuilder() override;
		std::unique_ptr<CommandPool> CreateCommandPool(RHI::QueueFamily family) override;
		std::unique_ptr<RHI::Buffer> CreateBuffer(RHI::BufferUsageFlags usage, UInt32 allocationSize, bool allowBufferMapping) override;
		std::size_t GetMinimumUniformBufferOffsetAlignment() const override;

		Vk::UploadContext& GetUploadContext();

	private:
		VkSurfaceKHR _surface;
		Vk::Instance* _vkInstance;
		std::optional<Vk::UploadContext> _uploadContext;
	};
}

#endif //CONCERTO_GRAPHICS_BACKEND_RHI_VULKAN_DEVICE_HPP