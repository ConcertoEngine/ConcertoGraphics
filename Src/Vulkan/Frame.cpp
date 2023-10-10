//
// Created by arthur on 13/09/2022.
//

#include "Concerto/Graphics/Frame.hpp"
#include "Concerto/Graphics/Vulkan/Wrapper/VulkanInitializer.hpp"
#include "Concerto/Graphics/Vulkan/Wrapper/CommandBuffer.hpp"
#include "Concerto/Graphics/Vulkan/Wrapper/CommandPool.hpp"
#include "Concerto/Graphics/Vulkan/Wrapper/Device.hpp"

namespace Concerto::Graphics
{
	using namespace Concerto::Graphics;
	Graphics::FrameData::FrameData(Device& device, bool signaled) :
		_presentSemaphore(device),
		_renderSemaphore(device),
		_renderFence(device, signaled),
		_commandPool(std::make_unique<CommandPool>(device, device.GetQueueFamilyIndex(Queue::Type::Graphics))),
		_mainCommandBuffer(std::make_unique<CommandBuffer>(device, *_commandPool->Get())),
		_indirectBuffer(MakeBuffer<VkDrawIndirectCommand>(device.GetAllocator(), MAX_OBJECTS, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_STORAGE_BUFFER_BIT |  VK_BUFFER_USAGE_INDIRECT_BUFFER_BIT, VMA_MEMORY_USAGE_CPU_TO_GPU)),
		_isResized(false)
	{
	}
}