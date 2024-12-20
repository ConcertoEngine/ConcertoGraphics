//
// Created by arthur on 03/09/2024.
//

#include <Concerto/Core/Cast.hpp>

#include "Concerto/Graphics/RHI/Vulkan/VkRHICommandPool.hpp"
#include "Concerto/Graphics/RHI/Vulkan/VkRHICommandBuffer.hpp"

namespace cct::gfx::rhi
{
	VkRHICommandPool::VkRHICommandPool(VkRHIDevice& device, QueueFamily family) :
		vk::CommandPool(device, device.GetQueueFamilyIndex(static_cast<vk::Queue::Type>(family))), //fixme?
		_device(device)
	{
	}

	std::unique_ptr<CommandBuffer> VkRHICommandPool::AllocateCommandBuffer(CommandBufferUasge usage)
	{
		return std::make_unique<VkRHICommandBuffer>(_device, *this);
	}
}