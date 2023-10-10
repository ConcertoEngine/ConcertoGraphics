//
// Created by arthur on 14/06/22.
//


#include <stdexcept>
#include <utility>

#include "Concerto/Graphics/Vulkan/Wrapper/CommandBuffer.hpp"
#include "Concerto/Graphics/Vulkan/Wrapper/Fence.hpp"
#include "Concerto/Graphics/Vulkan/Wrapper/Device.hpp"
#include "Concerto/Graphics/Vulkan/Wrapper/VulkanInitializer.hpp"
#include "Concerto/Graphics/Vulkan/Wrapper/Buffer.hpp"
#include "Concerto/Graphics/Vulkan/Wrapper/Pipeline.hpp"
#include "Concerto/Graphics/Vulkan/Wrapper/PipelineLayout.hpp"
#include "Concerto/Graphics/Vulkan/Wrapper/CommandPool.hpp"
#include "Concerto/Graphics/Vulkan/Wrapper/Queue.hpp"
#include "Concerto/Graphics/Vulkan/Utils.hpp"

namespace Concerto::Graphics
{
	CommandBuffer::CommandBuffer(Device& device, VkCommandPool commandPool) : _device(&device),
																			  _commandPool(commandPool)
	{
		VkCommandBufferAllocateInfo info = {};
		info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
		info.pNext = nullptr;
		info.commandPool = _commandPool;
		info.commandBufferCount = 1;
		info.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;

		if (vkAllocateCommandBuffers(*_device->Get(), &info, &_commandBuffer) != VK_SUCCESS)
		{
			throw std::runtime_error("Failed to allocate command buffer");
		}
	}

	CommandBuffer::~CommandBuffer()
	{
		if(_commandBuffer == VK_NULL_HANDLE)
			return;
		_device->WaitIdle();
		vkFreeCommandBuffers(*_device->Get(), _commandPool, 1, &_commandBuffer);
	}

	CommandBuffer::CommandBuffer(CommandBuffer&& other) noexcept
	{
		_device = std::exchange(other._device, nullptr);
		_commandPool = std::exchange(other._commandPool, VK_NULL_HANDLE);
		_commandBuffer = std::exchange(other._commandBuffer, nullptr);
	}

	CommandBuffer& CommandBuffer::operator=(CommandBuffer&& other) noexcept
	{
		_device = std::exchange(other._device, nullptr);
		_commandPool = std::exchange(other._commandPool, VK_NULL_HANDLE);
		_commandBuffer = std::exchange(other._commandBuffer, nullptr);
		return *this;
	}

	VkCommandBuffer CommandBuffer::Get() const
	{
		assert(_commandBuffer != VK_NULL_HANDLE);
		return _commandBuffer;
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
		vkCmdBindPipeline(_commandBuffer, pipelineBindPoint, *pipeline.Get());
	}

	void CommandBuffer::BindPipeline(VkPipelineBindPoint pipelineBindPoint, VkPipeline pipeline)
	{
		vkCmdBindPipeline(_commandBuffer, pipelineBindPoint, pipeline);
	}

	void CommandBuffer::Draw(UInt32 vertexCount, UInt32 instanceCount, UInt32 firstVertex,
			UInt32 firstInstance)
	{
		vkCmdDraw(_commandBuffer, vertexCount, instanceCount, firstVertex, firstInstance);
	}

	void CommandBuffer::DrawIndirect(Buffer& buffer, UInt32 offset, UInt32 drawCount, UInt32 stride)
	{
		vkCmdDrawIndirect(_commandBuffer, buffer._buffer, offset, drawCount, stride);
	}

	void CommandBuffer::BindVertexBuffers(const Buffer& buffer)
	{
		VkDeviceSize offset = 0;
		vkCmdBindVertexBuffers(_commandBuffer, 0, 1, &buffer._buffer, &offset);
	}

	void CommandBuffer::UpdatePushConstants(PipelineLayout& pipelineLayout, MeshPushConstants& meshPushConstants)
	{
		vkCmdPushConstants(_commandBuffer, *pipelineLayout.Get(), VK_SHADER_STAGE_VERTEX_BIT, 0,
				sizeof(MeshPushConstants), &meshPushConstants);
	}

	void CommandBuffer::UpdatePushConstants(VkPipelineLayout pipelineLayout, MeshPushConstants& meshPushConstants)
	{
		vkCmdPushConstants(_commandBuffer, pipelineLayout, VK_SHADER_STAGE_VERTEX_BIT, 0, sizeof(MeshPushConstants),
				&meshPushConstants);
	}

	void CommandBuffer::BindDescriptorSets(VkPipelineBindPoint pipelineBindPoint, VkPipelineLayout pipelineLayout,
			UInt32 firstSet, UInt32 descriptorSetCount, DescriptorSet& descriptorSet,
			UInt32 dynamicOffsets)
	{
		vkCmdBindDescriptorSets(_commandBuffer, pipelineBindPoint, pipelineLayout, firstSet, descriptorSetCount,
				descriptorSet.Get(), 1, &dynamicOffsets);
	}

	void CommandBuffer::BindDescriptorSets(VkPipelineBindPoint pipelineBindPoint, VkPipelineLayout pipelineLayout,
			UInt32 firstSet, UInt32 descriptorSetCount, DescriptorSet& descriptorSet)
	{
		vkCmdBindDescriptorSets(_commandBuffer, pipelineBindPoint, pipelineLayout, firstSet, descriptorSetCount,
				descriptorSet.Get(), 0, nullptr);
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

		if (vkQueueSubmit(*queue.Get(), 1, &submitInfo, *fence.Get()) != VK_SUCCESS)
		{
			throw std::runtime_error("vkQueueSubmit fail");
		}
		fence.Wait(9999999999);
		fence.Reset();
		commandPool.Reset();
	}

	void CommandBuffer::CopyBuffer(Buffer& src, Buffer& dest, std::size_t size)
	{
		VkBufferCopy copyRegion = {};
		copyRegion.srcOffset = 0;
		copyRegion.dstOffset = 0;
		copyRegion.size = size;
		vkCmdCopyBuffer(_commandBuffer, src._buffer, dest._buffer, 1, &copyRegion);
	}

	void CommandBuffer::SetViewport(VkViewport viewport)
	{
		vkCmdSetViewport(_commandBuffer, 0, 1, &viewport);
	}

	void CommandBuffer::SetScissor(VkRect2D scissor)
	{
		vkCmdSetScissor(_commandBuffer, 0, 1, &scissor);
	}
}