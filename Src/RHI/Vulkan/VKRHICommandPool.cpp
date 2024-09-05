//
// Created by arthur on 03/09/2024.
//

#include <Concerto/Core/Cast.hpp>

#include "Concerto/Graphics/RHI/Vulkan/VkRHICommandPool.hpp"
#include "Concerto/Graphics/RHI/Vulkan/VkRHICommandBuffer.hpp"

namespace Concerto::Graphics::RHI
{
	VkRHICommandPool::VkRHICommandPool(VkRHIDevice& device, QueueFamily family) :
		Vk::CommandPool(device, device.GetQueueFamilyIndex(static_cast<Vk::Queue::Type>(family))), //fixme?
		_device(device)
	{
	}

	std::unique_ptr<CommandBuffer> VkRHICommandPool::AllocateCommandBuffer(CommandBufferUasge usage)
	{
		return std::make_unique<VkRHICommandBuffer>(_device, *this);
	}
}