//
// Created by arthur on 14/06/22.
//

#ifndef CONCERTOGRAPHICS_COMMANDBUFFER_HPP
#define CONCERTOGRAPHICS_COMMANDBUFFER_HPP

#include "vulkan/vulkan.h"
#include "Pipeline.hpp"
#include "AllocatedBuffer.hpp"
#include "PipelineLayout.hpp"
#include "MeshPushConstants.hpp"

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

		VkCommandBuffer get() const;

		void reset();

		void begin();

		void end();

		void beginRenderPass(VkRenderPassBeginInfo info);

		void endRenderPass();

		void bindPipeline(VkPipelineBindPoint pipelineBindPoint, Pipeline& pipeline);

		void bindPipeline(VkPipelineBindPoint pipelineBindPoint, VkPipeline pipeline);

		void bindDescriptorSets(VkPipelineBindPoint pipelineBindPoint, VkPipelineLayout pipelineLayout,
				std::uint32_t firstSet, std::uint32_t descriptorSetCount, DescriptorSet& descriptorSet,
				std::uint32_t dynamicOffsets);
		void bindDescriptorSets(VkPipelineBindPoint pipelineBindPoint, VkPipelineLayout pipelineLayout,
				std::uint32_t firstSet, std::uint32_t descriptorSetCount, DescriptorSet& descriptorSet);

		void bindVertexBuffers(const AllocatedBuffer& buffer);

		void updatePushConstants(PipelineLayout& pipelineLayout, MeshPushConstants& meshPushConstants);

		void updatePushConstants(VkPipelineLayout pipelineLayout, MeshPushConstants& meshPushConstants);

		void draw(std::uint32_t vertexCount, std::uint32_t instanceCount, std::uint32_t firstVertex,
				std::uint32_t firstInstance);

	private:
		VkDevice _device;
		VkCommandPool _commandPool;
		VkCommandBuffer _commandBuffer;
	};
} // namespace Concerto::Graphics::Wrapper

#endif //CONCERTOGRAPHICS_COMMANDBUFFER_HPP
