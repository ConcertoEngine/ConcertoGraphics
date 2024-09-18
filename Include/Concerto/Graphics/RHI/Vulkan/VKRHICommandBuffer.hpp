//
// Created by arthur on 03/09/2024.
//

#ifndef CONCERTO_GRAPHICS_RHI_VKRHICOMMANDBUFFER_HPP
#define CONCERTO_GRAPHICS_RHI_VKRHICOMMANDBUFFER_HPP

#include "Concerto/Graphics/RHI/CommandBuffer.hpp"
#include "Concerto/Graphics/RHI/Vulkan/VkRHIDevice.hpp"
#include "Concerto/Graphics/Backend/Vulkan/Wrapper/CommandBuffer.hpp"

namespace Concerto::Graphics::RHI
{
	class CONCERTO_GRAPHICS_RHI_BASE_API VkRHICommandBuffer final: public RHI::CommandBuffer, public Vk::CommandBuffer
	{
	public:
		VkRHICommandBuffer(VkRHIDevice& device, Vk::CommandPool& commandPool);

		void Begin() override;
		void End() override;
		void Submit() override;
		void Reset() override;
		void SetViewport(const Viewport& viewport) override;
		void SetScissor(const Rect2D& scissor) override;
		void BeginRenderPass(const RHI::RenderPass& renderPass, const RHI::FrameBuffer& frameBuffer, const Vector3f& clearColor) override;
		void EndRenderPass() override;
		void BindMaterial(const MaterialInfo& material) override;
		void BindVertexBuffer(const RHI::Buffer& buffer) override;
		void Draw(UInt32 vertexCount, UInt32 instanceCount, UInt32 firstVertex, UInt32 firstInstance) override;

	private:
		VkPipeline _lastBoundedPipeline = {};
	};
}

#endif //CONCERTO_GRAPHICS_RHI_VKRHICOMMANDBUFFER_HPP