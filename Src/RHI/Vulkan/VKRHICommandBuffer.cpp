//
// Created by arthur on 03/09/2024.
//

#include <Concerto/Core/Cast.hpp>

#include "Concerto/Graphics/RHI/Vulkan/VkRHICommandBuffer.hpp"

#include "Concerto/Graphics/Backend/Vulkan/Wrapper/VulkanInitializer.hpp"
#include "Concerto/Graphics/RHI/Vulkan/VKRHIRenderPass.hpp"
#include "Concerto/Graphics/RHI/Vulkan/VkRHISwapChain.hpp"

namespace Concerto::Graphics::RHI
{
	VkRHICommandBuffer::VkRHICommandBuffer(VkRHIDevice& device, Vk::CommandPool& commandPool) :
		Vk::CommandBuffer(commandPool.AllocateCommandBuffer())
	{
	}

	void VkRHICommandBuffer::Begin()
	{
		Vk::CommandBuffer::Begin();
	}

	void VkRHICommandBuffer::End()
	{
		Vk::CommandBuffer::End();
	}

	void VkRHICommandBuffer::Submit()
	{
	}

	void VkRHICommandBuffer::Reset()
	{
		Vk::CommandBuffer::Reset();
	}

	void VkRHICommandBuffer::SetViewport(const Viewport& viewport)
	{
		VkViewport vkViewport = {
			.x = viewport.x,
			.y = viewport.y,
			.width = viewport.width,
			.height = viewport.height
		};
		Vk::CommandBuffer::SetViewport(vkViewport);
	}

	void VkRHICommandBuffer::SetScissor(const Rect2D& scissor)
	{
		VkRect2D vkScissor = {
			.offset = {
				.x = scissor.x,
				.y = scissor.x
			},
			.extent = {
				.width = scissor.width,
				.height = scissor.height
			}
		};
		Vk::CommandBuffer::SetScissor(vkScissor);
	}

	void VkRHICommandBuffer::BeginRenderPass(const RHI::RenderPass& renderPass, const RHI::SwapChain& swapChain, const Vector3f& clearColor)
	{
		const VkRHIRenderPass& vkRenderPass = Cast<const VkRHIRenderPass&>(renderPass);
		const VkRHISwapChain& vkSwapChain = Cast<const VkRHISwapChain&>(swapChain);

		VkExtent2D extent = {
			.width = vkSwapChain.GetExtent().X(),
			.height = vkSwapChain.GetExtent().Y()
		};
		
		VkClearValue depthClear;
		depthClear.depthStencil.depth = 1.f;
		std::array clearValues = {
					VkClearValue  {
					.color = {
						.float32 = {	clearColor.X(), clearColor.Y(), clearColor.Z(), 1.f}
					}
				},
			VkClearValue {
				.depthStencil = 1.f
			}
		};
		VkRenderPassBeginInfo renderPassInfo = VulkanInitializer::RenderPassBeginInfo(*vkRenderPass.Get(), extent, *vkSwapChain.GetCurrentFrameBuffer().Get());
		renderPassInfo.clearValueCount = static_cast<UInt32>(clearValues.size());
		renderPassInfo.pClearValues = clearValues.data();

		Vk::CommandBuffer::BeginRenderPass(renderPassInfo);
	}

	void VkRHICommandBuffer::EndRenderPass()
	{
		Vk::CommandBuffer::EndRenderPass();
	}
}
