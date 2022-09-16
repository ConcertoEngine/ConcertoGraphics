//
// Created by arthur on 14/06/22.
//

#ifndef CONCERTOGRAPHICS_COMMANDBUFFER_HPP
#define CONCERTOGRAPHICS_COMMANDBUFFER_HPP

#include <functional>
#include "vulkan/vulkan.h"
#include "Pipeline.hpp"
#include "AllocatedBuffer.hpp"
#include "PipelineLayout.hpp"
#include "MeshPushConstants.hpp"
#include "CommandPool.hpp"
#include "Queue.hpp"

namespace Concerto::Graphics::Wrapper
{
	class CommandBuffer
	{
	public:
		explicit CommandBuffer(VkDevice device, VkCommandPool commandPool);

		CommandBuffer(CommandBuffer&&) = default;

		CommandBuffer(const CommandBuffer&) = delete;

		CommandBuffer& operator=(CommandBuffer&&) = default;

		CommandBuffer& operator=(const CommandBuffer&) = delete;

		~CommandBuffer();

		VkCommandBuffer Get() const;

		void Reset();

		void Begin();

		void End();

		void BeginRenderPass(VkRenderPassBeginInfo info);

		void EndRenderPass();

		void BindPipeline(VkPipelineBindPoint pipelineBindPoint, Pipeline& pipeline);

		void BindPipeline(VkPipelineBindPoint pipelineBindPoint, VkPipeline pipeline);

		void BindDescriptorSets(VkPipelineBindPoint pipelineBindPoint, VkPipelineLayout pipelineLayout,
				std::uint32_t firstSet, std::uint32_t descriptorSetCount, DescriptorSet& descriptorSet,
				std::uint32_t dynamicOffsets);

		void BindDescriptorSets(VkPipelineBindPoint pipelineBindPoint, VkPipelineLayout pipelineLayout,
				std::uint32_t firstSet, std::uint32_t descriptorSetCount, DescriptorSet& descriptorSet);

		void BindVertexBuffers(const AllocatedBuffer& buffer);

		void UpdatePushConstants(PipelineLayout& pipelineLayout, MeshPushConstants& meshPushConstants);

		void UpdatePushConstants(VkPipelineLayout pipelineLayout, MeshPushConstants& meshPushConstants);

		void Draw(std::uint32_t vertexCount, std::uint32_t instanceCount, std::uint32_t firstVertex,
				std::uint32_t firstInstance);

		void ImmediateSubmit(Fence& fence, CommandPool& commandPool, Queue& queue,
				std::function<void(CommandBuffer&)>&& function);

		void CopyBuffer(AllocatedBuffer& src, AllocatedBuffer& dest, std::size_t size);

	private:
		VkDevice _device;
		VkCommandPool _commandPool;
		VkCommandBuffer _commandBuffer;
	};
} // namespace Concerto::Graphics::Wrapper

#endif //CONCERTOGRAPHICS_COMMANDBUFFER_HPP
