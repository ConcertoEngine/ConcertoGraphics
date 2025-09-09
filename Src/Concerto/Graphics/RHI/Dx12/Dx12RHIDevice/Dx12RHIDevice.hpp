//
// Created by arthur on 01/09/2025.
//

#ifndef CONCERTO_GRAPHICS_RHI_DX12_DX12RHIDEVICE_HPP
#define CONCERTO_GRAPHICS_RHI_DX12_DX12RHIDEVICE_HPP

#include "Concerto/Graphics/Backend/Dx12/Wrapper/Device/Device.hpp"
#include "Concerto/Graphics/RHI/Device.hpp"

namespace cct::gfx::dx12
{
	class PhysicalDevice;
}

namespace cct::gfx::rhi
{
	class CONCERTO_GRAPHICS_RHI_BASE_API Dx12RHIDevice : public rhi::Device, public dx12::Device
	{
	public:
		Dx12RHIDevice(ID3D12Device& device, dx12::PhysicalDevice& physicalDevice);
		~Dx12RHIDevice() override = default;

		std::unique_ptr<SwapChain> CreateSwapChain(Window& window, PixelFormat pixelFormat = PixelFormat::BGRA8_SRGB, PixelFormat depthPixelFormat = PixelFormat::D32f) override;
		std::unique_ptr<RenderPass> CreateRenderPass(std::span<RenderPass::Attachment> attachments, std::span<RenderPass::SubPassDescription> subPassDescriptions, std::span<RenderPass::SubPassDependency> subPassDependencies) override;
		std::unique_ptr<FrameBuffer> CreateFrameBuffer(UInt32 width, UInt32 height, const RenderPass& renderPass, const std::vector<std::unique_ptr<Texture>>& attachments) override;
		std::unique_ptr<FrameBuffer> CreateFrameBuffer(UInt32 width, UInt32 height, const RenderPass& renderPass, const std::vector<std::unique_ptr<TextureView>>& attachments) override;
		std::unique_ptr<MaterialBuilder> CreateMaterialBuilder(const Vector2u& windowExtent) override;
		std::unique_ptr<TextureBuilder> CreateTextureBuilder() override;
		std::unique_ptr<CommandPool> CreateCommandPool(rhi::QueueFamily family, CommandBufferUsage usage) override;
		std::unique_ptr<Buffer> CreateBuffer(rhi::BufferUsageFlags usage, UInt32 allocationSize, bool allowBufferMapping) override;
		std::size_t GetMinimumUniformBufferOffsetAlignment() const override;
		std::unique_ptr<GpuMesh> CreateMesh(const std::string& meshPath, rhi::MaterialBuilder& materialBuilder, const RenderPass& renderPass) override;
	};
}

#endif //CONCERTO_GRAPHICS_RHI_DX12_DX12RHIDEVICE_HPP