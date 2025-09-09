//
// Created by arthur on 01/09/2025.
//

#include "Concerto/Graphics/RHI/Dx12/Dx12RHISwapChain/Dx12RHISwapChain.hpp"
#include "Concerto/Graphics/RHI/Dx12/Dx12RHIDevice/Dx12RHIDevice.hpp"

namespace cct::gfx::rhi
{
	Dx12RHISwapChain::Dx12RHISwapChain(rhi::Dx12RHIDevice& device, Window& window, PixelFormat pixelFormat, PixelFormat depthPixelFormat) :
		rhi::SwapChain(pixelFormat, depthPixelFormat),
		dx12::SwapChain(device, window)
	{
	}

	RenderPass* Dx12RHISwapChain::GetRenderPass()
	{
		return nullptr;
	}

	Vector2u Dx12RHISwapChain::GetExtent() const
	{
		return dx12::SwapChain::GetExtent();
	}

	UInt32 Dx12RHISwapChain::GetImageCount() const
	{
		return dx12::SwapChain::ImageCount;
	}

	rhi::Frame& Dx12RHISwapChain::AcquireFrame()
	{
		return *reinterpret_cast<Frame*>(nullptr);
	}

	void Dx12RHISwapChain::WaitAll() const
	{
	}
}
