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

namespace cct::gfx::rhi
{
	class CONCERTO_GRAPHICS_RHI_BASE_API VkRHIDevice final : public rhi::Device, public vk::Device
	{
	public:
		VkRHIDevice(vk::PhysicalDevice& physicalDevice, vk::Instance& instance);

		// overrides
		std::unique_ptr<SwapChain> CreateSwapChain(Window& window, PixelFormat pixelFormat = PixelFormat::BGRA8_SRGB, PixelFormat depthPixelFormat = PixelFormat::D32f) override;
		std::unique_ptr<RenderPass> CreateRenderPass(std::span<RenderPass::Attachment> attachments, std::span<RenderPass::SubPassDescription> subPassDescriptions, std::span<rhi::RenderPass::SubPassDependency> subPassDependencies) override;
		std::unique_ptr<FrameBuffer> CreateFrameBuffer(UInt32 width, UInt32 height, const rhi::RenderPass& renderPass, const std::vector<std::unique_ptr<rhi::Texture>>& attachments) override;
		std::unique_ptr<FrameBuffer> CreateFrameBuffer(UInt32 width, UInt32 height, const RenderPass& renderPass, const std::vector<std::unique_ptr<TextureView>>& attachments) override;
		std::unique_ptr<MaterialBuilder> CreateMaterialBuilder(const Vector2u& windowExtent) override;
		std::unique_ptr<TextureBuilder> CreateTextureBuilder() override;
		std::unique_ptr<CommandPool> CreateCommandPool(rhi::QueueFamily family) override;
		std::unique_ptr<rhi::Buffer> CreateBuffer(rhi::BufferUsageFlags usage, UInt32 allocationSize, bool allowBufferMapping) override;
		std::size_t GetMinimumUniformBufferOffsetAlignment() const override;

		vk::UploadContext& GetUploadContext();
		vk::Instance& GetVkInstance();
	private:
		VkSurfaceKHR _surface;
		vk::Instance* _vkInstance;
		std::optional<vk::UploadContext> _uploadContext;
	};
}

#endif //CONCERTO_GRAPHICS_BACKEND_RHI_VULKAN_DEVICE_HPP