//
// Created by arthur on 01/09/2025.
//

#ifndef CONCERTO_GRAPHICS_RHI_DX12_DX12RHISWAPCHAIN_HPP
#define CONCERTO_GRAPHICS_RHI_DX12_DX12RHISWAPCHAIN_HPP

#include "Concerto/Graphics/RHI/SwapChain.hpp"
#include "Concerto/Graphics/Backend/Dx12/Wrapper/SwapChain/SwapChain.hpp"

namespace cct::gfx::rhi
{
	class Dx12RHIDevice;

	class CONCERTO_GRAPHICS_RHI_BASE_API Dx12RHISwapChain : public rhi::SwapChain, public dx12::SwapChain
	{
	public:
		Dx12RHISwapChain(rhi::Dx12RHIDevice& device, Window& window, PixelFormat pixelFormat, PixelFormat depthPixelFormat);

		RenderPass* GetRenderPass() override;
		Vector2u GetExtent() const override;
		UInt32 GetImageCount() const override;
		rhi::Frame& AcquireFrame() override;
		void WaitAll() const override;
	};
}

#endif //CONCERTO_GRAPHICS_RHI_DX12_DX12RHISWAPCHAIN_HPP