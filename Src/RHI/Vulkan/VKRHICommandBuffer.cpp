//
// Created by arthur on 03/09/2024.
//

#include "Concerto/Graphics/RHI/Vulkan/VKRHICommandBuffer.hpp"

namespace Concerto::Graphics::RHI
{
	VKRHICommandBuffer::VKRHICommandBuffer(VkRHIDevice& device, VkCommandPool commandPool) :
		Vk::CommandBuffer(device, commandPool)
	{
	}
}
