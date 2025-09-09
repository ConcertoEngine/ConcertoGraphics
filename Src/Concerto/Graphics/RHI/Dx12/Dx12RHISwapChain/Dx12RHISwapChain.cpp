//
// Created by arthur on 01/09/2025.
//

#include "Concerto/Graphics/RHI/Dx12/Dx12RHISwapChain/Dx12RHISwapChain.hpp"
#include "Concerto/Graphics/RHI/Dx12/Dx12RHIDevice/Dx12RHIDevice.hpp"

namespace cct::gfx::rhi
{
	Dx12RHISwapChain::Dx12RHISwapChain(rhi::Dx12RHIDevice& device, Window& window, PixelFormat pixelFormat, PixelFormat depthPixelFormat) :
		rhi::SwapChain(pixelFormat, depthPixelFormat),
		dx12::SwapChain(device, window),
		m_renderPass(std::make_unique<RenderPass>())
	{
		
	}

	RenderPass* Dx12RHISwapChain::GetRenderPass()
	{
		return m_renderPass.get();
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

	Dx12RHISwapChain::SwapChainFrame::SwapChainFrame(Dx12RHISwapChain& owner): m_renderFence()
	{
	}

	void Dx12RHISwapChain::SwapChainFrame::Present()
	{
	}

	rhi::CommandBuffer& Dx12RHISwapChain::SwapChainFrame::GetCommandBuffer()
	{
		throw;
	}

	std::size_t Dx12RHISwapChain::SwapChainFrame::GetCurrentFrameIndex()
	{
		return 0;
	}

	rhi::FrameBuffer& Dx12RHISwapChain::SwapChainFrame::GetFrameBuffer()
	{
		throw;
	}

	void Dx12RHISwapChain::SwapChainFrame::SetNextImageIndex(UInt32 imageIndex)
	{
	}

	void Dx12RHISwapChain::SwapChainFrame::Wait() const
	{
		m_renderFence.Wait();
	}

	const dx12::Fence& Dx12RHISwapChain::SwapChainFrame::GetRenderFence() const
	{
		return m_renderFence;
	}
}
