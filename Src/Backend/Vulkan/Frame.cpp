//
// Created by arthur on 13/09/2022.
//

#include "Concerto/Graphics/Frame.hpp"
#include "Concerto/Graphics/Backend/Vulkan/Wrapper/CommandBuffer.hpp"
#include "Concerto/Graphics/Backend/Vulkan/Wrapper/CommandPool.hpp"
#include "Concerto/Graphics/Backend/Vulkan/Wrapper/Device.hpp"

namespace Concerto::Graphics::Vk
{
	FrameData::FrameData(Device& device, bool signaled) :
		presentSemaphore(device),
		renderSemaphore(device),
		renderFence(device, signaled),
		commandPool(device, device.GetQueueFamilyIndex(Queue::Type::Graphics)),
		commandBuffer(commandPool.AllocateCommandBuffer()),
		indirectBuffer(MakeBuffer<VkDrawIndirectCommand>(device.GetAllocator(), MAX_OBJECTS, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_STORAGE_BUFFER_BIT |  VK_BUFFER_USAGE_INDIRECT_BUFFER_BIT, VMA_MEMORY_USAGE_CPU_TO_GPU)),
		isResized(false)
	{
	}
}