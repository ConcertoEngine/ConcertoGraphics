//
// Created by arthur on 03/09/2024.
//

#ifndef CONCERTO_GRAPHICS_RHI_VKRHICOMMANDBUFFER_HPP
#define CONCERTO_GRAPHICS_RHI_VKRHICOMMANDBUFFER_HPP

#include "Concerto/Graphics/RHI/CommandBuffer.hpp"
#include "Concerto/Graphics/RHI/Vulkan/VkRHIDevice/VkRHIDevice.hpp"
#include "Concerto/Graphics/Backend/Vulkan/Wrapper/CommandBuffer/CommandBuffer.hpp"

namespace cct::gfx::rhi
{
	class CONCERTO_GRAPHICS_RHI_BASE_API VkRHICommandBuffer final: public rhi::CommandBuffer, public vk::CommandBuffer
	{
	public:
		VkRHICommandBuffer(VkRHIDevice& device, vk::CommandPool& commandPool);

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

	private:
		VkPipeline m_lastBoundedPipeline = {};
	};
}

#endif //CONCERTO_GRAPHICS_RHI_VKRHICOMMANDBUFFER_HPP