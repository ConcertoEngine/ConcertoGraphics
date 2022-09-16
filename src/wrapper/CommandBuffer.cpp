//
// Created by arthur on 14/06/22.
//


#include "wrapper/CommandBuffer.hpp"
#include <stdexcept>
#include "wrapper/VulkanInitializer.hpp"

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

	VkCommandBuffer CommandBuffer::Get() const
	{
		return _commandBuffer;
	}

	CommandBuffer::~CommandBuffer()
	{
		vkFreeCommandBuffers(_device, _commandPool, 1, &_commandBuffer);
		_commandBuffer = VK_NULL_HANDLE;
	}

	void CommandBuffer::Reset()
	{
		if (vkResetCommandBuffer(_commandBuffer, 0) != VK_SUCCESS)
		{
			throw std::runtime_error("vkResetCommandBuffer fail");
		}
	}

	void CommandBuffer::Begin()
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

	void CommandBuffer::End()
	{
		if (vkEndCommandBuffer(_commandBuffer) != VK_SUCCESS)
		{
			throw std::runtime_error("vkEndCommandBuffer fail");
		}
	}

	void CommandBuffer::BeginRenderPass(VkRenderPassBeginInfo info)
	{
		vkCmdBeginRenderPass(_commandBuffer, &info, VK_SUBPASS_CONTENTS_INLINE);
	}

	void CommandBuffer::EndRenderPass()
	{
		vkCmdEndRenderPass(_commandBuffer);
	}

	void CommandBuffer::BindPipeline(VkPipelineBindPoint pipelineBindPoint, Pipeline& pipeline)
	{
		vkCmdBindPipeline(_commandBuffer, pipelineBindPoint, pipeline.Get());
	}

	void CommandBuffer::BindPipeline(VkPipelineBindPoint pipelineBindPoint, VkPipeline pipeline)
	{
		vkCmdBindPipeline(_commandBuffer, pipelineBindPoint, pipeline);
	}

	void CommandBuffer::Draw(std::uint32_t vertexCount, std::uint32_t instanceCount, std::uint32_t firstVertex,
			std::uint32_t firstInstance)
	{
		vkCmdDraw(_commandBuffer, vertexCount, instanceCount, firstVertex, firstInstance);
	}

	void CommandBuffer::BindVertexBuffers(const AllocatedBuffer& buffer)
	{
		VkDeviceSize offset = 0;
		vkCmdBindVertexBuffers(_commandBuffer, 0, 1, &buffer._buffer, &offset);
	}

	void CommandBuffer::UpdatePushConstants(PipelineLayout& pipelineLayout, MeshPushConstants& meshPushConstants)
	{
		vkCmdPushConstants(_commandBuffer, pipelineLayout.Get(), VK_SHADER_STAGE_VERTEX_BIT, 0,
				sizeof(MeshPushConstants), &meshPushConstants);
	}

	void CommandBuffer::UpdatePushConstants(VkPipelineLayout pipelineLayout, MeshPushConstants& meshPushConstants)
	{
		vkCmdPushConstants(_commandBuffer, pipelineLayout, VK_SHADER_STAGE_VERTEX_BIT, 0, sizeof(MeshPushConstants),
				&meshPushConstants);
	}

	void CommandBuffer::BindDescriptorSets(VkPipelineBindPoint pipelineBindPoint, VkPipelineLayout pipelineLayout,
			std::uint32_t firstSet, std::uint32_t descriptorSetCount, DescriptorSet& descriptorSet,
			std::uint32_t dynamicOffsets)
	{
		auto vkDescriptorSet = descriptorSet.Get();
		vkCmdBindDescriptorSets(_commandBuffer, pipelineBindPoint, pipelineLayout, firstSet, descriptorSetCount,
				&vkDescriptorSet, 1, &dynamicOffsets);
	}

	void CommandBuffer::BindDescriptorSets(VkPipelineBindPoint pipelineBindPoint, VkPipelineLayout pipelineLayout,
			std::uint32_t firstSet, std::uint32_t descriptorSetCount, DescriptorSet& descriptorSet)
	{
		auto vkDescriptorSet = descriptorSet.Get();
		vkCmdBindDescriptorSets(_commandBuffer, pipelineBindPoint, pipelineLayout, firstSet, descriptorSetCount,
				&vkDescriptorSet, 0, nullptr);
	}

	void CommandBuffer::ImmediateSubmit(Fence& fence, CommandPool& commandPool, Queue& queue,
			std::function<void(CommandBuffer&)>&& function)
	{
		VkSubmitInfo submitInfo = VulkanInitializer::SubmitInfo(&_commandBuffer);
		Begin();
		{
			function(*this);
		}
		End();

		if (vkQueueSubmit(*queue.Get(), 1, &submitInfo, fence.Get()) != VK_SUCCESS)
		{
			throw std::runtime_error("vkQueueSubmit fail");
		}
		fence.wait(9999999999);
		fence.reset();
		commandPool.reset();
	}

	void CommandBuffer::CopyBuffer(AllocatedBuffer& src, AllocatedBuffer& dest, std::size_t size)
	{
		VkBufferCopy copyRegion = {};
		copyRegion.srcOffset = 0;
		copyRegion.dstOffset = 0;
		copyRegion.size = size;
		vkCmdCopyBuffer(_commandBuffer, src._buffer, dest._buffer, 1, &copyRegion);
	}
}