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
#include "Concerto/Graphics/Vulkan/Wrapper/DescriptorSet.hpp"
#include "Concerto/Graphics/Vulkan/Utils.hpp"

namespace Concerto::Graphics
{
	CommandBuffer::CommandBuffer(Device& device, const VkCommandPool commandPool) : _device(&device),
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
			CONCERTO_ASSERT_FALSE;
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
		CONCERTO_ASSERT(_commandBuffer != VK_NULL_HANDLE);
		return _commandBuffer;
	}

	void CommandBuffer::Reset() const
	{
		if (vkResetCommandBuffer(_commandBuffer, 0) != VK_SUCCESS)
		{
			CONCERTO_ASSERT_FALSE;
			throw std::runtime_error("vkResetCommandBuffer fail");
		}
	}

	void CommandBuffer::Begin() const
	{
		VkCommandBufferBeginInfo cmdBeginInfo = {};

		cmdBeginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
		cmdBeginInfo.pNext = nullptr;
		cmdBeginInfo.pInheritanceInfo = nullptr;
		cmdBeginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

		if (vkBeginCommandBuffer(_commandBuffer, &cmdBeginInfo) != VK_SUCCESS)
		{
			CONCERTO_ASSERT_FALSE;
			throw std::runtime_error("vkBeginCommandBuffer fail");
		}
	}

	void CommandBuffer::End() const
	{
		if (vkEndCommandBuffer(_commandBuffer) != VK_SUCCESS)
		{
			CONCERTO_ASSERT_FALSE;
			throw std::runtime_error("vkEndCommandBuffer fail");
		}
	}

	void CommandBuffer::BeginRenderPass(const VkRenderPassBeginInfo& info) const
	{
		vkCmdBeginRenderPass(_commandBuffer, &info, VK_SUBPASS_CONTENTS_INLINE);
	}

	void CommandBuffer::EndRenderPass() const
	{
		vkCmdEndRenderPass(_commandBuffer);
	}

	void CommandBuffer::BindPipeline(const VkPipelineBindPoint pipelineBindPoint, const Pipeline& pipeline) const
	{
		vkCmdBindPipeline(_commandBuffer, pipelineBindPoint, *pipeline.Get());
	}

	void CommandBuffer::BindPipeline(const VkPipelineBindPoint pipelineBindPoint, const VkPipeline pipeline) const
	{
		vkCmdBindPipeline(_commandBuffer, pipelineBindPoint, pipeline);
	}

	void CommandBuffer::Draw(const UInt32 vertexCount, const UInt32 instanceCount, const UInt32 firstVertex,
	                         const UInt32 firstInstance) const
	{
		vkCmdDraw(_commandBuffer, vertexCount, instanceCount, firstVertex, firstInstance);
	}

	void CommandBuffer::DrawIndirect(const Buffer& buffer, const UInt32 offset, const UInt32 drawCount, const UInt32 stride) const
	{
		vkCmdDrawIndirect(_commandBuffer, *buffer.Get(), offset, drawCount, stride);
	}

	void CommandBuffer::BindVertexBuffers(const Buffer& buffer) const
	{
		VkDeviceSize offset = 0;
		vkCmdBindVertexBuffers(_commandBuffer, 0, 1,  buffer.Get(), &offset);
	}

	void CommandBuffer::UpdatePushConstants(const PipelineLayout& pipelineLayout, const MeshPushConstants& meshPushConstants) const
	{
		vkCmdPushConstants(_commandBuffer, *pipelineLayout.Get(), VK_SHADER_STAGE_VERTEX_BIT, 0,
				sizeof(MeshPushConstants), &meshPushConstants);
	}

	void CommandBuffer::UpdatePushConstants(const VkPipelineLayout pipelineLayout, const MeshPushConstants& meshPushConstants) const
	{
		vkCmdPushConstants(_commandBuffer, pipelineLayout, VK_SHADER_STAGE_VERTEX_BIT, 0, sizeof(MeshPushConstants),
				&meshPushConstants);
	}

	void CommandBuffer::BindDescriptorSets(const VkPipelineBindPoint pipelineBindPoint, const VkPipelineLayout pipelineLayout,
	                                       const UInt32 firstSet, const UInt32 descriptorSetCount, const DescriptorSet& descriptorSet,
	                                       const UInt32 dynamicOffsets) const
	{
		vkCmdBindDescriptorSets(_commandBuffer, pipelineBindPoint, pipelineLayout, firstSet, descriptorSetCount,
				descriptorSet.Get(), 1, &dynamicOffsets);
	}

	void CommandBuffer::BindDescriptorSets(const VkPipelineBindPoint pipelineBindPoint, const VkPipelineLayout pipelineLayout,
	                                       const UInt32 firstSet, const UInt32 descriptorSetCount, const DescriptorSet& descriptorSet) const
	{
		vkCmdBindDescriptorSets(_commandBuffer, pipelineBindPoint, pipelineLayout, firstSet, descriptorSetCount,
				descriptorSet.Get(), 0, nullptr);
	}

	void CommandBuffer::BindDescriptorSets(const VkPipelineBindPoint pipelineBindPoint, const VkPipelineLayout pipelineLayout,
	                                       const std::span<DescriptorSet> descriptorSets) const
	{
		std::vector<VkDescriptorSet> vkDescriptorSets;
		vkDescriptorSets.reserve(descriptorSets.size());
		for (const auto& descriptorSet : descriptorSets)
			vkDescriptorSets.push_back(*descriptorSet.Get());
		vkCmdBindDescriptorSets(_commandBuffer, pipelineBindPoint, pipelineLayout, 0, static_cast<UInt32>(vkDescriptorSets.size()),
			vkDescriptorSets.data(), 0, nullptr);
	}

	void CommandBuffer::BindDescriptorSets(const VkPipelineBindPoint pipelineBindPoint, const VkPipelineLayout pipelineLayout,
	                                       const std::span<DescriptorSetPtr> descriptorSets) const
	{
		std::vector<VkDescriptorSet> vkDescriptorSets;
		vkDescriptorSets.reserve(descriptorSets.size());
		for (const auto& descriptorSet : descriptorSets)
			vkDescriptorSets.push_back(*descriptorSet->Get());
		vkCmdBindDescriptorSets(_commandBuffer, pipelineBindPoint, pipelineLayout, 0, vkDescriptorSets.size(),
			vkDescriptorSets.data(), 0, nullptr);
	}

	void CommandBuffer::ImmediateSubmit(Fence& fence, CommandPool& commandPool, const Queue& queue,
	                                    std::function<void(CommandBuffer&)>&& function)
	{
		const VkSubmitInfo submitInfo = VulkanInitializer::SubmitInfo(&_commandBuffer);
		Begin();
		{
			function(*this);
		}
		End();

		if (vkQueueSubmit(*queue.Get(), 1, &submitInfo, *fence.Get()) != VK_SUCCESS)
		{
			CONCERTO_ASSERT_FALSE;
			throw std::runtime_error("vkQueueSubmit fail");
		}
		fence.Wait(9999999999);
		fence.Reset();
		commandPool.Reset();
	}

	void CommandBuffer::CopyBuffer(const Buffer& src, const Buffer& dest, const std::size_t size) const
	{
		const VkBufferCopy copyRegion = {
			.srcOffset = 0,
			.dstOffset = 0,
			.size = size
		};
		vkCmdCopyBuffer(_commandBuffer, *src.Get(), *dest.Get(), 1, &copyRegion);
	}

	void CommandBuffer::SetViewport(const VkViewport& viewport) const
	{
		vkCmdSetViewport(_commandBuffer, 0, 1, &viewport);
	}

	void CommandBuffer::SetScissor(const VkRect2D scissor) const
	{
		vkCmdSetScissor(_commandBuffer, 0, 1, &scissor);
	}
}