//
// Created by arthur on 14/06/22.
//


#include "wrapper/CommandBuffer.hpp"
#include <stdexcept>

namespace Concerto::Graphics::Wrapper
{
	CommandBuffer::CommandBuffer(VkDevice device, VkCommandPool commandPool) : _device(device),
																			   _commandPool(commandPool)
	{
		VkCommandBufferAllocateInfo info = {};
		info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
		info.pNext = nullptr;
		info.commandPool = _commandPool;
		info.commandBufferCount = 1;
		info.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;

		if (vkAllocateCommandBuffers(_device, &info, &_commandBuffer) != VK_SUCCESS)
		{
			throw std::runtime_error("Failed to allocate command buffer");
		}
	}

	VkCommandBuffer CommandBuffer::get() const
	{
		return _commandBuffer;
	}

	CommandBuffer::~CommandBuffer()
	{
		vkFreeCommandBuffers(_device, _commandPool, 1, &_commandBuffer);
	}
}