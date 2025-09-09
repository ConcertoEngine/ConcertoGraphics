//
// Created by arthur on 01/09/2025.
//

#include "Concerto/Graphics/RHI/Dx12/Dx12RHICommandBuffer/Dx12RHICommandBuffer.hpp"

namespace cct::gfx::rhi
{
	Dx12RHICommandBuffer::Dx12RHICommandBuffer(dx12::CommandAllocator& owner, D3D12_COMMAND_LIST_TYPE type) :
		rhi::CommandBuffer(),
		dx12::CommandList(owner, type)
	{
	}

	void Dx12RHICommandBuffer::Begin()
	{
	}

	void Dx12RHICommandBuffer::End()
	{
	}

	void Dx12RHICommandBuffer::Submit()
	{
	}

	void Dx12RHICommandBuffer::Reset()
	{
	}

	void Dx12RHICommandBuffer::SetViewport(const Viewport& viewport)
	{
	}

	void Dx12RHICommandBuffer::SetScissor(const Rect2D& scissor)
	{
	}

	void Dx12RHICommandBuffer::BeginRenderPass(const rhi::RenderPass& renderPass, const rhi::FrameBuffer& frameBuffer, const Vector3f& clearColor)
	{
	}

	void Dx12RHICommandBuffer::EndRenderPass()
	{
	}

	void Dx12RHICommandBuffer::BindMaterial(const MaterialInfo& material)
	{
	}

	void Dx12RHICommandBuffer::BindVertexBuffer(const rhi::Buffer& buffer)
	{
	}

	void Dx12RHICommandBuffer::Draw(UInt32 vertexCount, UInt32 instanceCount, UInt32 firstVertex, UInt32 firstInstance)
	{
	}
}
