//
// Created by arthur on 03/09/2024.
//

#include <Concerto/Core/Cast.hpp>

#include "Concerto/Graphics/Backend/Vulkan/Wrapper/Pipeline.hpp"
#include "Concerto/Graphics/Backend/Vulkan/Wrapper/VulkanInitializer.hpp"

#include "Concerto/Graphics/RHI/Vulkan/VkRHICommandBuffer.hpp"
#include "Concerto/Graphics/RHI/Vulkan/VkMaterial.hpp"
#include "Concerto/Graphics/RHI/Vulkan/VkRHIBuffer.hpp"
#include "Concerto/Graphics/RHI/Vulkan/VKRHIRenderPass.hpp"
#include "Concerto/Graphics/RHI/Vulkan/VkRHISwapChain.hpp"
#include "Concerto/Graphics/RHI/Vulkan/VkRHIBuffer.hpp"
#include "Concerto/Graphics/RHI/Vulkan/VKRHIFrameBuffer.hpp"

namespace cct::gfx::rhi
{
	VkRHICommandBuffer::VkRHICommandBuffer(VkRHIDevice& device, vk::CommandPool& commandPool) :
		vk::CommandBuffer(commandPool.AllocateCommandBuffer(VK_COMMAND_BUFFER_LEVEL_PRIMARY))
	{
	}

	void VkRHICommandBuffer::Begin()
	{
		vk::CommandBuffer::Begin();
	}

	void VkRHICommandBuffer::End()
	{
		vk::CommandBuffer::End();
	}

	void VkRHICommandBuffer::Submit()
	{
	}

	void VkRHICommandBuffer::Reset()
	{
		vk::CommandBuffer::Reset();
	}

	void VkRHICommandBuffer::SetViewport(const Viewport& viewport)
	{
		VkViewport vkViewport = {
			.x = viewport.x,
			.y = viewport.y,
			.width = viewport.width,
			.height = viewport.height,
			.minDepth = viewport.minDepth,
			.maxDepth = viewport.maxDepth
		};
		vk::CommandBuffer::SetViewport(vkViewport);
	}

	void VkRHICommandBuffer::SetScissor(const Rect2D& scissor)
	{
		const VkRect2D vkScissor = {
			.offset = {
				.x = scissor.x,
				.y = scissor.x
			},
			.extent = {
				.width = scissor.width,
				.height = scissor.height
			}
		};
		vk::CommandBuffer::SetScissor(vkScissor);
	}

	void VkRHICommandBuffer::BeginRenderPass(const rhi::RenderPass& renderPass, const rhi::FrameBuffer& frameBuffer, const Vector3f& clearColor)
	{
		CCT_GFX_AUTO_PROFILER_SCOPE();

		const VkRHIRenderPass& vkRenderPass = Cast<const VkRHIRenderPass&>(renderPass);
		const VkRHIFrameBuffer& vkRhiFrameBuffer = Cast<const VkRHIFrameBuffer&>(frameBuffer);

		const VkExtent2D extent = {
			.width = frameBuffer.GetWidth(),
			.height = frameBuffer.GetHeight()
		};

		const std::array clearValues = {
					VkClearValue  {
					.color = {
						.float32 = {	clearColor.X(), clearColor.Y(), clearColor.Z(), 0.f}
					}
				},
			VkClearValue {
				.depthStencil = {1.f, 0}
			}
		};
		VkRenderPassBeginInfo renderPassInfo = VulkanInitializer::RenderPassBeginInfo(*vkRenderPass.Get(), extent, *vkRhiFrameBuffer.Get());
		renderPassInfo.clearValueCount = static_cast<UInt32>(clearValues.size());
		renderPassInfo.pClearValues = clearValues.data();

		vk::CommandBuffer::BeginRenderPass(renderPassInfo);
	}

	void VkRHICommandBuffer::EndRenderPass()
	{
		CCT_GFX_AUTO_PROFILER_SCOPE();

		vk::CommandBuffer::EndRenderPass();
	}

	void VkRHICommandBuffer::BindMaterial(const MaterialInfo& material)
	{
		CCT_GFX_AUTO_PROFILER_SCOPE();

		const vk::VkMaterial& vkMaterial = Cast<const vk::VkMaterial&>(material);
		const VkPipeline pipeline = *vkMaterial.pipeline->Get();
		//if (_lastBoundedPipeline == pipeline)
		//	return;
		vk::CommandBuffer::BindPipeline(VK_PIPELINE_BIND_POINT_GRAPHICS, pipeline);
		//_lastBoundedPipeline = pipeline;
		auto cpy = vkMaterial.descriptorSets; //fixme
		vk::CommandBuffer::BindDescriptorSets(VK_PIPELINE_BIND_POINT_GRAPHICS, *vkMaterial.pipeline->GetPipelineLayout()->Get(), cpy);
	}

	void VkRHICommandBuffer::BindVertexBuffer(const rhi::Buffer& buffer)
	{
		CCT_GFX_AUTO_PROFILER_SCOPE();

		const VkRHIBuffer& vkBuffer = Cast<const VkRHIBuffer&>(buffer);
		vk::CommandBuffer::BindVertexBuffers(vkBuffer);
	}

	void VkRHICommandBuffer::Draw(UInt32 vertexCount, UInt32 instanceCount, UInt32 firstVertex, UInt32 firstInstance)
	{
		CCT_GFX_AUTO_PROFILER_SCOPE();

		vk::CommandBuffer::Draw(vertexCount, instanceCount, firstVertex, firstInstance);
	}
}
