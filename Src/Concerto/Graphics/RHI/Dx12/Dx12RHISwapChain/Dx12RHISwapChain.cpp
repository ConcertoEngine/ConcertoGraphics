//
// Created by arthur on 01/09/2025.
//

#include <Concerto/Core/Cast.hpp>

#include "Concerto/Graphics/RHI/Dx12/Dx12RHISwapChain/Dx12RHISwapChain.hpp"
#include "Concerto/Graphics/Backend/Dx12/Wrapper/CommandList/CommandList.hpp"
#include "Concerto/Graphics/RHI/Dx12/Dx12RHICommandBuffer/Dx12RHICommandBuffer.hpp"
#include "Concerto/Graphics/RHI/Dx12/Dx12RHIDevice/Dx12RHIDevice.hpp"

namespace cct::gfx::rhi
{
	Dx12RHISwapChain::Dx12RHISwapChain(rhi::Dx12RHIDevice& device, Window& window, PixelFormat pixelFormat, PixelFormat depthPixelFormat) :
		rhi::SwapChain(pixelFormat, depthPixelFormat),
		dx12::SwapChain(device, window),
		m_renderPass(std::make_unique<RenderPass>()),
		m_currentFrameIndex(0),
		m_commandPool(device, CommandBufferUsage::Primary, D3D12_COMMAND_LIST_TYPE_DIRECT)
	{
		m_frames.reserve(dx12::SwapChain::ImageCount);
		for (UINT32 i = 0; i < dx12::SwapChain::ImageCount; ++i)
			m_frames.emplace_back(*this, i);
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
		m_frames[m_currentFrameIndex].Wait();
		m_currentFrameIndex = (m_currentFrameIndex + 1) % GetImageCount();
		return m_frames[m_currentFrameIndex];
	}

	void Dx12RHISwapChain::WaitAll() const
	{
		for (auto& frame : m_frames)
			frame.GetRenderFence().Wait();
	}

	CommandPool& Dx12RHISwapChain::GetCommandPool()
	{
		return m_commandPool;
	}

	Dx12RHISwapChain::SwapChainFrame::SwapChainFrame(Dx12RHISwapChain& owner, UINT32 imageIndex) :
		m_renderFence(*owner.GetDevice()),
		m_owner(&owner),
		m_imageIndex(imageIndex),
		m_commandBuffer(Cast<Dx12RHICommandPool&>(owner.GetCommandPool()), D3D12_COMMAND_LIST_TYPE_DIRECT)
	{
	}

	void Dx12RHISwapChain::SwapChainFrame::Present()
	{
		std::array<ID3D12CommandList*, 1> commandLists = {
			m_commandBuffer.Get()
		};
		m_owner->GetCommandQueue()->ExecuteCommandLists(commandLists.size(), commandLists.data());
		CCT_GFX_FRAME_MARK;
	}

	rhi::CommandBuffer& Dx12RHISwapChain::SwapChainFrame::GetCommandBuffer()
	{
		return m_commandBuffer;
	}

	std::size_t Dx12RHISwapChain::SwapChainFrame::GetCurrentFrameIndex()
	{
		return m_imageIndex;
	}

	rhi::FrameBuffer& Dx12RHISwapChain::SwapChainFrame::GetFrameBuffer()
	{
		throw;
	}

	void Dx12RHISwapChain::SwapChainFrame::Wait() const
	{
		m_owner->GetCommandQueue()->Signal(m_renderFence.Get(), m_renderFence.GetCompletedValue());
		m_renderFence.Wait();
	}

	const dx12::Fence& Dx12RHISwapChain::SwapChainFrame::GetRenderFence() const
	{
		return m_renderFence;
	}
}
