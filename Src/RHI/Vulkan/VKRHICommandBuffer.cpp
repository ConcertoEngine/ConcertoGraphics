//
// Created by arthur on 03/09/2024.
//

#include "Concerto/Graphics/RHI/Vulkan/VkRHICommandBuffer.hpp"

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
}
