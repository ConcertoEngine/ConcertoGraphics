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

namespace cct::gfx::vk
{
	CommandBuffer::CommandBuffer(CommandPool& owner, VkCommandBufferLevel level) :
		Object(*owner.GetDevice()),
		_commandPool(&owner),
		_level(level)
	{
		VkCommandBufferAllocateInfo info = {};
		info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
		info.pNext = nullptr;
		info.commandPool = *owner.Get();
		info.commandBufferCount = 1;
		info.level = level;

		const VkResult result = _device->vkAllocateCommandBuffers(*_device->Get(), &info, &_handle);
		CCT_ASSERT(result == VK_SUCCESS, "ConcertoGraphics: vkAllocateCommandBuffers failed VkResult={}", static_cast<int>(result));
	}

	CommandBuffer::~CommandBuffer()
	{
		if(_handle == VK_NULL_HANDLE)
			return;
		if (_device)
		{
			_device->WaitIdle();
			_device->vkFreeCommandBuffers(*_device->Get(), *_commandPool->Get(), 1, &_handle);
		}
	}

	CommandBuffer::CommandBuffer(CommandBuffer&& other) noexcept :
		Object(std::move(other)),
		_commandPool(std::exchange(other._commandPool, VK_NULL_HANDLE)),
		_level(std::exchange(other._level, VK_COMMAND_BUFFER_LEVEL_MAX_ENUM))
	{
	}

	CommandBuffer& CommandBuffer::operator=(CommandBuffer&& other) noexcept
	{
		std::swap(_device, other._device);
		std::swap(_commandPool, other._commandPool);
		std::swap(_handle, other._handle);

		Object::operator=(std::move(other));
		return *this;
	}

	void CommandBuffer::Reset() const
	{
		CCT_GFX_AUTO_PROFILER_SCOPE();

		const VkResult result = _device->vkResetCommandBuffer(_handle, 0);
		CCT_ASSERT(result == VK_SUCCESS, "ConcertoGraphics: vkResetCommandBuffer VKResult={}", static_cast<int>(result));
	}

	void CommandBuffer::Begin() const
	{
		CCT_GFX_AUTO_PROFILER_SCOPE();

		VkCommandBufferBeginInfo cmdBeginInfo = {};

		cmdBeginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
		cmdBeginInfo.pNext = nullptr;
		cmdBeginInfo.pInheritanceInfo = nullptr;
		cmdBeginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

		VkCommandBufferInheritanceInfo commandBufferInheritanceInfo;
		if (_level == VK_COMMAND_BUFFER_LEVEL_SECONDARY)
		{
			commandBufferInheritanceInfo = {};
			commandBufferInheritanceInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_INHERITANCE_INFO;
			cmdBeginInfo.pInheritanceInfo = &commandBufferInheritanceInfo;
		}

		const VkResult result = _device->vkBeginCommandBuffer(_handle, &cmdBeginInfo);
		CCT_ASSERT(result == VK_SUCCESS, "ConcertoGraphics: vkBeginCommandBuffer failed VKResult={}", static_cast<int>(result));
#ifdef CCT_ENABLE_OBJECT_DEBUG
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
		CCT_GFX_AUTO_PROFILER_SCOPE();
#ifdef CCT_ENABLE_OBJECT_DEBUG
		if (_device->IsExtensionEnabled(VK_EXT_DEBUG_MARKER_EXTENSION_NAME))
		{
			_device->vkCmdDebugMarkerEndEXT(_handle);
		}
#endif
		const VkResult result = _device->vkEndCommandBuffer(_handle);
		CCT_ASSERT(result == VK_SUCCESS, "ConcertoGraphics: vkEndCommandBuffer failed VKResult={}", static_cast<int>(result));
	}

	void CommandBuffer::BeginRenderPass(const VkRenderPassBeginInfo& info) const
	{
		CCT_GFX_AUTO_PROFILER_SCOPE();

		_device->vkCmdBeginRenderPass(_handle, &info, VK_SUBPASS_CONTENTS_INLINE);
	}

	void CommandBuffer::EndRenderPass() const
	{
		CCT_GFX_AUTO_PROFILER_SCOPE();

		_device->vkCmdEndRenderPass(_handle);
	}

	void CommandBuffer::BindPipeline(const VkPipelineBindPoint pipelineBindPoint, const Pipeline& pipeline) const
	{
		CCT_GFX_AUTO_PROFILER_SCOPE();

		_device->vkCmdBindPipeline(_handle, pipelineBindPoint, *pipeline.Get());
	}

	void CommandBuffer::BindPipeline(const VkPipelineBindPoint pipelineBindPoint, const VkPipeline pipeline) const
	{
		CCT_GFX_AUTO_PROFILER_SCOPE();

		_device->vkCmdBindPipeline(_handle, pipelineBindPoint, pipeline);
	}

	void CommandBuffer::Draw(const UInt32 vertexCount, const UInt32 instanceCount, const UInt32 firstVertex, const UInt32 firstInstance) const
	{
		CCT_GFX_AUTO_PROFILER_SCOPE();

		_device->vkCmdDraw(_handle, vertexCount, instanceCount, firstVertex, firstInstance);
	}

	void CommandBuffer::DrawIndirect(const Buffer& buffer, const UInt32 offset, const UInt32 drawCount, const UInt32 stride) const
	{
		CCT_GFX_AUTO_PROFILER_SCOPE();

		_device->vkCmdDrawIndirect(_handle, *buffer.Get(), offset, drawCount, stride);
	}

	void CommandBuffer::BindVertexBuffers(const Buffer& buffer) const
	{
		CCT_GFX_AUTO_PROFILER_SCOPE();

		VkDeviceSize offset = 0;
		_device->vkCmdBindVertexBuffers(_handle, 0, 1,  buffer.Get(), &offset);
	}

	void CommandBuffer::UpdatePushConstants(const PipelineLayout& pipelineLayout, const MeshPushConstants& meshPushConstants) const
	{
		CCT_GFX_AUTO_PROFILER_SCOPE();

		_device->vkCmdPushConstants(_handle, *pipelineLayout.Get(), VK_SHADER_STAGE_VERTEX_BIT, 0,sizeof(MeshPushConstants), &meshPushConstants);
	}

	void CommandBuffer::UpdatePushConstants(const VkPipelineLayout pipelineLayout, const MeshPushConstants& meshPushConstants) const
	{
		CCT_GFX_AUTO_PROFILER_SCOPE();

		_device->vkCmdPushConstants(_handle, pipelineLayout, VK_SHADER_STAGE_VERTEX_BIT, 0, sizeof(MeshPushConstants), &meshPushConstants);
	}

	void CommandBuffer::BindDescriptorSets(const VkPipelineBindPoint pipelineBindPoint, const VkPipelineLayout pipelineLayout,
	                                       const UInt32 firstSet, const UInt32 descriptorSetCount, const DescriptorSet& descriptorSet,
	                                       const UInt32 dynamicOffsets) const
	{
		CCT_GFX_AUTO_PROFILER_SCOPE();

		_device->vkCmdBindDescriptorSets(_handle, pipelineBindPoint, pipelineLayout, firstSet, descriptorSetCount, descriptorSet.Get(), 1, &dynamicOffsets);
	}

	void CommandBuffer::BindDescriptorSets(const VkPipelineBindPoint pipelineBindPoint, const VkPipelineLayout pipelineLayout,
	                                       const UInt32 firstSet, const UInt32 descriptorSetCount, const DescriptorSet& descriptorSet) const
	{
		CCT_GFX_AUTO_PROFILER_SCOPE();

		_device->vkCmdBindDescriptorSets(_handle, pipelineBindPoint, pipelineLayout, firstSet, descriptorSetCount, descriptorSet.Get(), 0, nullptr);
	}

	void CommandBuffer::BindDescriptorSets(const VkPipelineBindPoint pipelineBindPoint, const VkPipelineLayout pipelineLayout,
	                                       const std::span<DescriptorSet> descriptorSets) const
	{
		CCT_GFX_AUTO_PROFILER_SCOPE();

		std::vector<VkDescriptorSet> vkDescriptorSets;
		vkDescriptorSets.reserve(descriptorSets.size());
		for (const auto& descriptorSet : descriptorSets)
			vkDescriptorSets.push_back(*descriptorSet.Get());
		_device->vkCmdBindDescriptorSets(_handle, pipelineBindPoint, pipelineLayout, 0, static_cast<UInt32>(vkDescriptorSets.size()), vkDescriptorSets.data(), 0, nullptr);
	}

	void CommandBuffer::BindDescriptorSets(const VkPipelineBindPoint pipelineBindPoint, const VkPipelineLayout pipelineLayout,
	                                       const std::span<DescriptorSetPtr> descriptorSets) const
	{
		CCT_GFX_AUTO_PROFILER_SCOPE();

		std::vector<VkDescriptorSet> vkDescriptorSets;
		vkDescriptorSets.reserve(descriptorSets.size());
		for (const auto& descriptorSet : descriptorSets)
			vkDescriptorSets.push_back(*descriptorSet->Get());
		_device->vkCmdBindDescriptorSets(_handle, pipelineBindPoint, pipelineLayout, 0, static_cast<UInt32>(vkDescriptorSets.size()), vkDescriptorSets.data(), 0, nullptr);
	}

	void CommandBuffer::ImmediateSubmit(const Fence& fence, const CommandPool& commandPool, const Queue& queue, std::function<void(CommandBuffer&)>&& function)
	{
		CCT_GFX_AUTO_PROFILER_SCOPE();

		Begin();
		{
			function(*this);
		}
		End();

		Submit(fence, commandPool, queue);
	}

	void CommandBuffer::Submit(const Fence& fence, const CommandPool& commandPool, const Queue& queue)
	{
		CCT_GFX_AUTO_PROFILER_SCOPE();
		const VkSubmitInfo submitInfo = VulkanInitializer::SubmitInfo(&_handle);
		const VkResult result = _device->vkQueueSubmit(*queue.Get(), 1, &submitInfo, *fence.Get());
		CCT_ASSERT(result == VK_SUCCESS, "ConcertoGraphics: vkQueueSubmit failed VKResult={}", static_cast<int>(result));

		fence.Wait(9999999999);
		fence.Reset();
		commandPool.Reset();
	}

	void CommandBuffer::ExecuteCommands(std::span<CommandBuffer> commandBuffers) const
	{
		CCT_GFX_AUTO_PROFILER_SCOPE();

		std::vector<VkCommandBuffer> vkCommandBuffers;
		vkCommandBuffers.reserve(commandBuffers.size());

		for (auto& cb : commandBuffers)
			vkCommandBuffers.push_back(*cb.Get());

		_device->vkCmdExecuteCommands(*Get(), vkCommandBuffers.size(), vkCommandBuffers.data());
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