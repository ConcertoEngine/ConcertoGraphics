//
// Created by arthur on 14/06/22.
//


#include "wrapper/CommandBuffer.hpp"
#include <stdexcept>
#include <iostream>
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
		_commandBuffer = VK_NULL_HANDLE;
	}

	void CommandBuffer::reset()
	{
		if (vkResetCommandBuffer(_commandBuffer, 0) != VK_SUCCESS)
		{
			throw std::runtime_error("vkResetCommandBuffer fail");
		}
	}

	void CommandBuffer::begin()
	{
		VkCommandBufferBeginInfo cmdBeginInfo = {};

		cmdBeginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
		cmdBeginInfo.pNext = nullptr;
		cmdBeginInfo.pInheritanceInfo = nullptr;
		cmdBeginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

		if (vkBeginCommandBuffer(_commandBuffer, &cmdBeginInfo) != VK_SUCCESS)
		{
			throw std::runtime_error("vkBeginCommandBuffer fail");
		}
	}

	void CommandBuffer::end()
	{
		if (vkEndCommandBuffer(_commandBuffer) != VK_SUCCESS)
		{
			throw std::runtime_error("vkEndCommandBuffer fail");
		}
	}

	void CommandBuffer::beginRenderPass(VkRenderPassBeginInfo info)
	{
		vkCmdBeginRenderPass(_commandBuffer, &info, VK_SUBPASS_CONTENTS_INLINE);
	}

	void CommandBuffer::endRenderPass()
	{
		vkCmdEndRenderPass(_commandBuffer);
	}

	void CommandBuffer::bindPipeline(VkPipelineBindPoint pipelineBindPoint, Pipeline& pipeline)
	{
		vkCmdBindPipeline(_commandBuffer, pipelineBindPoint, pipeline.get());
	}

	void CommandBuffer::draw(std::uint32_t vertexCount, std::uint32_t instanceCount, std::uint32_t firstVertex, std::uint32_t firstInstance)
	{
		vkCmdDraw(_commandBuffer, vertexCount, instanceCount, firstVertex, firstInstance);
	}
}