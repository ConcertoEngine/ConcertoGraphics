//
// Created by arthur on 01/09/2025.
//

#include "Concerto/Graphics/RHI/Dx12/Dx12RHIDevice/Dx12RHIDevice.hpp"
#include "Concerto/Graphics/RHI/Dx12/Dx12RHISwapChain/Dx12RHISwapChain.hpp"
//#include "Concerto/Graphics/RHI/Dx12/Dx12RHIRenderPass/Dx12RHIRenderPass.hpp"
//#include "Concerto/Graphics/RHI/Dx12/Dx12RHIFrameBuffer/Dx12RHIFrameBuffer.hpp"
//#include "Concerto/Graphics/RHI/Dx12/Dx12RHIMaterialBuilder/Dx12RHIMaterialBuilder.hpp"
//#include "Concerto/Graphics/RHI/Dx12/Dx12RHITextureBuilder/Dx12RHITextureBuilder.hpp"
#include "Concerto/Graphics/RHI/Dx12/Dx12RHICommandPool/Dx12RHICommandPool.hpp"
#include "Concerto/Graphics/RHI/Dx12/Dx12RHIBuffer/Dx12RHIBuffer.hpp"

namespace cct::gfx::rhi
{
	Dx12RHIDevice::Dx12RHIDevice(ID3D12Device& device, dx12::PhysicalDevice& physicalDevice) : rhi::Device(), dx12::Device(device, physicalDevice)
	{
	}

	std::unique_ptr<SwapChain> Dx12RHIDevice::CreateSwapChain(Window& window, PixelFormat pixelFormat, PixelFormat depthPixelFormat)
	{
		auto swapChain = std::make_unique<Dx12RHISwapChain>(*this, window, pixelFormat, depthPixelFormat);
		if (FAILED(swapChain->GetLastResult()))
		{
			CCT_ASSERT_FALSE("ConcertoGraphics: Error occured during swapchain creation error={}", swapChain->GetLastResult());
			return nullptr;
		}
		return swapChain;
	}

	std::unique_ptr<RenderPass> Dx12RHIDevice::CreateRenderPass(std::span<RenderPass::Attachment> attachments,
		std::span<RenderPass::SubPassDescription> subPassDescriptions, std::span<RenderPass::SubPassDependency> subPassDependencies)
	{
		return nullptr;
	}

	std::unique_ptr<FrameBuffer> Dx12RHIDevice::CreateFrameBuffer(UInt32 width, UInt32 height,
		const RenderPass& renderPass, const std::vector<std::unique_ptr<Texture>>& attachments)
	{
		return nullptr;
	}

	std::unique_ptr<FrameBuffer> Dx12RHIDevice::CreateFrameBuffer(UInt32 width, UInt32 height,
		const RenderPass& renderPass, const std::vector<std::unique_ptr<TextureView>>& attachments)
	{
		return nullptr;
	}

	std::unique_ptr<MaterialBuilder> Dx12RHIDevice::CreateMaterialBuilder(const Vector2u& windowExtent)
	{
		return nullptr;
	}

	std::unique_ptr<TextureBuilder> Dx12RHIDevice::CreateTextureBuilder()
	{
		return nullptr;
	}

	std::unique_ptr<CommandPool> Dx12RHIDevice::CreateCommandPool(rhi::QueueFamily family, CommandBufferUsage usage)
	{
		return nullptr;
	}

	std::unique_ptr<Buffer> Dx12RHIDevice::CreateBuffer(rhi::BufferUsageFlags usage, UInt32 allocationSize, bool allowBufferMapping)
	{
		return nullptr;
	}

	std::size_t Dx12RHIDevice::GetMinimumUniformBufferOffsetAlignment() const
	{
		return D3D12_CONSTANT_BUFFER_DATA_PLACEMENT_ALIGNMENT;
	}

	std::unique_ptr<GpuMesh> Dx12RHIDevice::CreateMesh(const std::string& meshPath, rhi::MaterialBuilder& materialBuilder, const RenderPass& renderPass)
	{
		return nullptr;
	}
}
