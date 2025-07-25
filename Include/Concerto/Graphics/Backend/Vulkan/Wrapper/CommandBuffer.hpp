//
// Created by arthur on 14/06/22.
//

#ifndef CONCERTO_GRAPHICS_COMMANDBUFFER_HPP
#define CONCERTO_GRAPHICS_COMMANDBUFFER_HPP

#include <functional>
#include <span>
#include <memory>

#include "Concerto/Graphics/Backend/Vulkan/Wrapper/Object.hpp"

namespace cct::gfx::vk
{
	class Device;
	class Fence;
	class Pipeline;
	class PipelineLayout;
	class CommandPool;
	class Queue;
	class Buffer;
	class MeshPushConstants;
	class DescriptorSet;

	class CONCERTO_GRAPHICS_VULKAN_BACKEND_API CommandBuffer : public Object<VkCommandBuffer>
	{
	public:
		CommandBuffer(CommandPool& owner, VkCommandBufferLevel level);

		~CommandBuffer();

		CommandBuffer(CommandBuffer&&) noexcept;

		CommandBuffer(const CommandBuffer&) = delete;

		CommandBuffer& operator=(CommandBuffer&&) noexcept;

		CommandBuffer& operator=(const CommandBuffer&) = delete;

		/**
		 * @brief Resets the command buffer.
		 */
		void Reset() const;

		/**
		 * @brief Begins recording commands.
		 */
		void Begin() const;

		/**
		 * @brief Ends recording commands.
		 */
		void End() const;

		/**
		 * @brief Begins a render pass.
		 * @param info The VkRenderPassBeginInfo containing information about the render pass to begin.
		 */
		void BeginRenderPass(const VkRenderPassBeginInfo& info) const;

		/**
		 * @brief Ends a render pass.
		 */
		void EndRenderPass() const;

		/**
		 * @brief Binds a pipeline to the command buffer.
		 * @param pipelineBindPoint The bind point of the pipeline.
		 * @param pipeline The Pipeline object to bind.
		 */
		void BindPipeline(VkPipelineBindPoint pipelineBindPoint, const Pipeline& pipeline) const;

		/**
		 * @brief Binds a pipeline to the command buffer.
		 * @param pipelineBindPoint The bind point of the pipeline.
		 * @param pipeline The VkPipeline to bind.
		 */
		void BindPipeline(VkPipelineBindPoint pipelineBindPoint, VkPipeline pipeline) const;

		/**
		 * @brief Binds descriptor sets to the command buffer.
		 * @param pipelineBindPoint The bind point of the pipeline.
		 * @param pipelineLayout The VkPipelineLayout associated with the pipeline.
		 * @param firstSet The index of the first descriptor set to bind.
		 * @param descriptorSetCount The number of descriptor sets to bind.
		 * @param descriptorSet The DescriptorSet object to bind.
		 * @param dynamicOffsets The dynamic offset value for each descriptor set.
		 */
		void BindDescriptorSets(VkPipelineBindPoint pipelineBindPoint, VkPipelineLayout pipelineLayout,
				UInt32 firstSet, UInt32 descriptorSetCount, const DescriptorSet& descriptorSet,
				UInt32 dynamicOffsets) const;

		/**
		 * @brief Binds descriptor sets to the command buffer.
		 * @param pipelineBindPoint The bind point of the pipeline.
		 * @param pipelineLayout The VkPipelineLayout associated with the pipeline.
		 * @param firstSet The index of the first descriptor set to bind.
		 * @param descriptorSetCount The number of descriptor sets to bind.
		 * @param descriptorSet The DescriptorSet object to bind.
		 */
		void BindDescriptorSets(VkPipelineBindPoint pipelineBindPoint, VkPipelineLayout pipelineLayout,
				UInt32 firstSet, UInt32 descriptorSetCount, const DescriptorSet& descriptorSet) const;

		/**
		 * @brief Binds descriptor sets to the command buffer.
		 * @param pipelineBindPoint The bind point of the pipeline.
		 * @param pipelineLayout The VkPipelineLayout associated with the pipeline.
		 * @param descriptorSets The DescriptorSet object to bind.
		 */
		void BindDescriptorSets(VkPipelineBindPoint pipelineBindPoint, VkPipelineLayout pipelineLayout, std::span<DescriptorSet> descriptorSets) const;


		/**
		 * @brief Binds descriptor sets to the command buffer.
		 * @param pipelineBindPoint The bind point of the pipeline.
		 * @param pipelineLayout The VkPipelineLayout associated with the pipeline.
		 * @param descriptorSets The DescriptorSet object to bind.
		 */
		void BindDescriptorSets(VkPipelineBindPoint pipelineBindPoint, VkPipelineLayout pipelineLayout, std::span<std::shared_ptr<DescriptorSet>> descriptorSets) const;

		/**
		 * @brief Binds vertex buffers to the command buffer.
		 * @param buffer The Buffer object to bind.
		 */
		void BindVertexBuffers(const Buffer& buffer) const;

		/**
		 * @brief Updates push constants in the command buffer.
		 * @param pipelineLayout The PipelineLayout associated with the pipeline.
		 * @param meshPushConstants The MeshPushConstants object to update.
		 */
		void UpdatePushConstants(const PipelineLayout& pipelineLayout, const MeshPushConstants& meshPushConstants) const;

		/**
		 * @brief Updates push constants in the command buffer.
		 * @param pipelineLayout The VkPipelineLayout associated with the pipeline.
		 * @param meshPushConstants The MeshPushConstants object to update.
		 */
		void UpdatePushConstants(VkPipelineLayout pipelineLayout, const MeshPushConstants& meshPushConstants) const;

		/**
		 * @brief Draws vertex data on the command buffer.
		 * @param vertexCount The number of vertices to draw.
		 * @param instanceCount The number of instances to draw.
		 * @param firstVertex The index of the first vertex to draw.
		 * @param firstInstance The instance ID of the first instance to draw.
		 */
		void Draw(UInt32 vertexCount, UInt32 instanceCount, UInt32 firstVertex,
				UInt32 firstInstance) const;

		/**
		 * @brief Draws indexed vertex data on the command buffer.
		 * @param buffer The Buffer object containing the index data.
		 * @param offset The offset into the buffer to start reading index data from.
		 * @param drawCount The number of draws to perform.
		 * @param stride The stride between each draw.
		 */
		void DrawIndirect(const Buffer& buffer, UInt32 offset, UInt32 drawCount, UInt32 stride) const;

		/**
		 * @brief Submits the command buffer to the queue, and waits for the fence to signal.
		 * @param fence The Fence object to wait on.
		 * @param commandPool The CommandPool object associated with the command buffer.
		 * @param queue The Queue object to submit the command buffer to.
		 * @param function The function to call before submitting the command buffer.
		 */
		void ImmediateSubmit(const Fence& fence, const CommandPool& commandPool, const Queue& queue,
				std::function<void(CommandBuffer&)>&& function);

		void Submit(const Fence& fence, const CommandPool& commandPool, const Queue& queue);

		void ExecuteCommands(std::span<CommandBuffer> commandBuffers) const;

		/**
	     * @brief Copies data from one buffer to another.
	     * @param src The source Buffer object.
	     * @param dest The destination Buffer object.
	     * @param size The size of the data to copy.
	     * @param srcOffset Is the starting offset in bytes from the start of src
	     * @param dstOffset Is the starting offset in bytes from the start of dst
	     */
		void CopyBuffer(const Buffer& src, const Buffer& dest, std::size_t size, std::size_t srcOffset = 0, std::size_t dstOffset = 0) const;

		/**
		 * @brief Sets the viewport on the command buffer.
		 * @param viewport The VkViewport to set.
		 */
		void SetViewport(const VkViewport& viewport) const;

		/**
		 * @brief Sets the scissor on the command buffer.
		 * @param scissor The VkRect2D to set.
		 */
		void SetScissor(VkRect2D scissor) const;

	private:
		vk::CommandPool* _commandPool;
		VkCommandBufferLevel _level;
	};
} // namespace cct::gfx::vk

#endif //CONCERTO_GRAPHICS_COMMANDBUFFER_HPP
