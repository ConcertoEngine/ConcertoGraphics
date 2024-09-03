//
// Created by arthur on 03/09/2024.
//

#include <Concerto/Core/Cast.hpp>

#include "Concerto/Graphics/RHI/Vulkan/VKRHICommandPool.hpp"
#include "Concerto/Graphics/RHI/Vulkan/VKRHICommandBuffer.hpp"

namespace Concerto::Graphics::RHI
{
	VKRHICommandPool::VKRHICommandPool(VkRHIDevice& device, QueueFamily family) :
		Vk::CommandPool(device, device.GetQueueFamilyIndex(static_cast<Vk::Queue::Type>(family))), //fixme?
		_device(device)
	{
	}

	std::unique_ptr<CommandBuffer> VKRHICommandPool::AllocateCommandBuffer(CommandBufferUasge usage)
	{
		return std::make_unique<VKRHICommandBuffer>(_device, *this->Get());
	}
}