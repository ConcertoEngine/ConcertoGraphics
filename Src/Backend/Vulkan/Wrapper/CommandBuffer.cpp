//
// Created by arthur on 14/06/22.
//

#include <utility>

#include "Concerto/Graphics/Backend/Vulkan/Wrapper/CommandBuffer.hpp"
#include "Concerto/Graphics/Backend/Vulkan/Wrapper/Fence.hpp"
#include "Concerto/Graphics/Backend/Vulkan/Wrapper/Device.hpp"
#include "Concerto/Graphics/Backend/Vulkan/Wrapper/VulkanInitializer.hpp"
#include "Concerto/Graphics/Backend/Vulkan/Wrapper/Buffer.hpp"
#include "Concerto/Graphics/Backend/Vulkan/Wrapper/Pipeline.hpp"
#include "Concerto/Graphics/Backend/Vulkan/Wrapper/PipelineLayout.hpp"
#include "Concerto/Graphics/Backend/Vulkan/Wrapper/CommandPool.hpp"
#include "Concerto/Graphics/Backend/Vulkan/Wrapper/Queue.hpp"
#include "Concerto/Graphics/Backend/Vulkan/Wrapper/DescriptorSet.hpp"
#include "Concerto/Graphics/Backend/Vulkan/Utils.hpp"

namespace Concerto::Graphics::Vk
{
	CommandBuffer::CommandBuffer(Device& device, Vk::CommandPool& owner, VkCommandBuffer commandBuffer) :
		Object(device),
		_commandPool(&owner)
	{
		_handle = commandBuffer;
	}

	CommandBuffer::~CommandBuffer()
	{
		if(_handle == VK_NULL_HANDLE)
			return;
		_device->WaitIdle();
		_device->vkFreeCommandBuffers(*_device->Get(), *_commandPool->Get(), 1, &_handle);
	}

	CommandBuffer::CommandBuffer(CommandBuffer&& other) noexcept :
		Object(*std::exchange(other._device, VK_NULL_HANDLE))
	{
		_commandPool = std::exchange(other._commandPool, VK_NULL_HANDLE);
	}

	CommandBuffer& CommandBuffer::operator=(CommandBuffer&& other) noexcept
	{
		_device = std::exchange(other._device, nullptr);
		_commandPool = std::exchange(other._commandPool, VK_NULL_HANDLE);
		_handle = std::exchange(other._handle, nullptr);
		return *this;
	}

	void CommandBuffer::Reset() const
	{
		const VkResult result = _device->vkResetCommandBuffer(_handle, 0);
		CONCERTO_ASSERT(result == VK_SUCCESS, "ConcertoGraphics: vkResetCommandBuffer VKResult={}", static_cast<int>(result));
	}

	void CommandBuffer::Begin() const
	{
		VkCommandBufferBeginInfo cmdBeginInfo = {};

		cmdBeginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
		cmdBeginInfo.pNext = nullptr;
		cmdBeginInfo.pInheritanceInfo = nullptr;
		cmdBeginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

		const VkResult result = _device->vkBeginCommandBuffer(_handle, &cmdBeginInfo);
		CONCERTO_ASSERT(result == VK_SUCCESS, "ConcertoGraphics: vkBeginCommandBuffer failed VKResult={}", static_cast<int>(result));
#ifdef CONCERTO_DEBUG
		if (_device->IsExtensionEnabled(VK_EXT_DEBUG_MARKER_EXTENSION_NAME))
		{
			VkDebugMarkerMarkerInfoEXT markerInfo = {
				.sType = VK_STRUCTURE_TYPE_DEBUG_MARKER_MARKER_INFO_EXT,
				.pNext = nullptr,
				.pMarkerName = ObjectDebug::GetDebugName().data(),
				.color = {1.f, 1.f, 0.f}
			};
			_device->vkCmdDebugMarkerBeginEXT(_handle, &markerInfo);
		}
#endif
	}

	void CommandBuffer::End() const
	{
#ifdef CONCERTO_DEBUG
		if (_device->IsExtensionEnabled(VK_EXT_DEBUG_MARKER_EXTENSION_NAME))
		{
			_device->vkCmdDebugMarkerEndEXT(_handle);
		}
#endif
		const VkResult result = _device->vkEndCommandBuffer(_handle);
		CONCERTO_ASSERT(result == VK_SUCCESS, "ConcertoGraphics: vkEndCommandBuffer failed VKResult={}", static_cast<int>(result));
	}

	void CommandBuffer::BeginRenderPass(const VkRenderPassBeginInfo& info) const
	{
		_device->vkCmdBeginRenderPass(_handle, &info, VK_SUBPASS_CONTENTS_INLINE);
	}

	void CommandBuffer::EndRenderPass() const
	{
		_device->vkCmdEndRenderPass(_handle);
	}

	void CommandBuffer::BindPipeline(const VkPipelineBindPoint pipelineBindPoint, const Pipeline& pipeline) const
	{
		_device->vkCmdBindPipeline(_handle, pipelineBindPoint, *pipeline.Get());
	}

	void CommandBuffer::BindPipeline(const VkPipelineBindPoint pipelineBindPoint, const VkPipeline pipeline) const
	{
		_device->vkCmdBindPipeline(_handle, pipelineBindPoint, pipeline);
	}

	void CommandBuffer::Draw(const UInt32 vertexCount, const UInt32 instanceCount, const UInt32 firstVertex, const UInt32 firstInstance) const
	{
		_device->vkCmdDraw(_handle, vertexCount, instanceCount, firstVertex, firstInstance);
	}

	void CommandBuffer::DrawIndirect(const Buffer& buffer, const UInt32 offset, const UInt32 drawCount, const UInt32 stride) const
	{
		_device->vkCmdDrawIndirect(_handle, *buffer.Get(), offset, drawCount, stride);
	}

	void CommandBuffer::BindVertexBuffers(const Buffer& buffer) const
	{
		VkDeviceSize offset = 0;
		_device->vkCmdBindVertexBuffers(_handle, 0, 1,  buffer.Get(), &offset);
	}

	void CommandBuffer::UpdatePushConstants(const PipelineLayout& pipelineLayout, const MeshPushConstants& meshPushConstants) const
	{
		_device->vkCmdPushConstants(_handle, *pipelineLayout.Get(), VK_SHADER_STAGE_VERTEX_BIT, 0,sizeof(MeshPushConstants), &meshPushConstants);
	}

	void CommandBuffer::UpdatePushConstants(const VkPipelineLayout pipelineLayout, const MeshPushConstants& meshPushConstants) const
	{
		_device->vkCmdPushConstants(_handle, pipelineLayout, VK_SHADER_STAGE_VERTEX_BIT, 0, sizeof(MeshPushConstants), &meshPushConstants);
	}

	void CommandBuffer::BindDescriptorSets(const VkPipelineBindPoint pipelineBindPoint, const VkPipelineLayout pipelineLayout,
	                                       const UInt32 firstSet, const UInt32 descriptorSetCount, const DescriptorSet& descriptorSet,
	                                       const UInt32 dynamicOffsets) const
	{
		_device->vkCmdBindDescriptorSets(_handle, pipelineBindPoint, pipelineLayout, firstSet, descriptorSetCount, descriptorSet.Get(), 1, &dynamicOffsets);
	}

	void CommandBuffer::BindDescriptorSets(const VkPipelineBindPoint pipelineBindPoint, const VkPipelineLayout pipelineLayout,
	                                       const UInt32 firstSet, const UInt32 descriptorSetCount, const DescriptorSet& descriptorSet) const
	{
		_device->vkCmdBindDescriptorSets(_handle, pipelineBindPoint, pipelineLayout, firstSet, descriptorSetCount, descriptorSet.Get(), 0, nullptr);
	}

	void CommandBuffer::BindDescriptorSets(const VkPipelineBindPoint pipelineBindPoint, const VkPipelineLayout pipelineLayout,
	                                       const std::span<DescriptorSet> descriptorSets) const
	{
		std::vector<VkDescriptorSet> vkDescriptorSets;
		vkDescriptorSets.reserve(descriptorSets.size());
		for (const auto& descriptorSet : descriptorSets)
			vkDescriptorSets.push_back(*descriptorSet.Get());
		_device->vkCmdBindDescriptorSets(_handle, pipelineBindPoint, pipelineLayout, 0, static_cast<UInt32>(vkDescriptorSets.size()), vkDescriptorSets.data(), 0, nullptr);
	}

	void CommandBuffer::BindDescriptorSets(const VkPipelineBindPoint pipelineBindPoint, const VkPipelineLayout pipelineLayout,
	                                       const std::span<DescriptorSetPtr> descriptorSets) const
	{
		std::vector<VkDescriptorSet> vkDescriptorSets;
		vkDescriptorSets.reserve(descriptorSets.size());
		for (const auto& descriptorSet : descriptorSets)
			vkDescriptorSets.push_back(*descriptorSet->Get());
		_device->vkCmdBindDescriptorSets(_handle, pipelineBindPoint, pipelineLayout, 0, vkDescriptorSets.size(), vkDescriptorSets.data(), 0, nullptr);
	}

	void CommandBuffer::ImmediateSubmit(Fence& fence, CommandPool& commandPool, const Queue& queue,
	                                    std::function<void(CommandBuffer&)>&& function)
	{
		const VkSubmitInfo submitInfo = VulkanInitializer::SubmitInfo(&_handle);
		Begin();
		{
			function(*this);
		}
		End();

		const VkResult result = _device->vkQueueSubmit(*queue.Get(), 1, &submitInfo, *fence.Get());
		CONCERTO_ASSERT(result == VK_SUCCESS, "ConcertoGraphics: vkQueueSubmit failed VKResult={}", static_cast<int>(result));

		fence.Wait(9999999999);
		fence.Reset();
		commandPool.Reset();
	}

	void CommandBuffer::CopyBuffer(const Buffer& src, const Buffer& dest, const std::size_t size, std::size_t srcOffset, std::size_t dstOffset) const
	{
		const VkBufferCopy copyRegion = {
			.srcOffset = srcOffset,
			.dstOffset = dstOffset,
			.size = size
		};
		_device->vkCmdCopyBuffer(_handle, *src.Get(), *dest.Get(), 1, &copyRegion);
	}

	void CommandBuffer::SetViewport(const VkViewport& viewport) const
	{
		_device->vkCmdSetViewport(_handle, 0, 1, &viewport);
	}

	void CommandBuffer::SetScissor(const VkRect2D scissor) const
	{
		_device->vkCmdSetScissor(_handle, 0, 1, &scissor);
	}
}