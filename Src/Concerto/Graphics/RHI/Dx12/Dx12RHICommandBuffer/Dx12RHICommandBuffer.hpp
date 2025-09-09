//
// Created by arthur on 01/09/2025.
//

#ifndef CONCERTO_GRAPHICS_RHI_DX12_DX12RHICOMMANDBUFFER_HPP
#define CONCERTO_GRAPHICS_RHI_DX12_DX12RHICOMMANDBUFFER_HPP

#include "Concerto/Graphics/RHI/CommandBuffer.hpp"

namespace cct::gfx::rhi
{
	class CONCERTO_GRAPHICS_RHI_BASE_API Dx12RHICommandBuffer : public rhi::CommandBuffer
	{
	public:
		Dx12RHICommandBuffer() = default;

		void Begin() override;
		void End() override;
		void Submit() override;
		void Reset() override;
		void SetViewport(const Viewport& viewport) override;
		void SetScissor(const Rect2D& scissor) override;
		void BeginRenderPass(const rhi::RenderPass& renderPass, const rhi::FrameBuffer& frameBuffer, const Vector3f& clearColor) override;
		void EndRenderPass() override;
		void BindMaterial(const MaterialInfo& material) override;
		void BindVertexBuffer(const rhi::Buffer& buffer) override;
		void Draw(UInt32 vertexCount, UInt32 instanceCount, UInt32 firstVertex, UInt32 firstInstance) override;
	};
}

#endif //CONCERTO_GRAPHICS_RHI_DX12_DX12RHICOMMANDBUFFER_HPP