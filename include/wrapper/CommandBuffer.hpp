//
// Created by arthur on 14/06/22.
//

#ifndef CONCERTOGRAPHICS_COMMANDBUFFER_HPP
#define CONCERTOGRAPHICS_COMMANDBUFFER_HPP

#include "vulkan/vulkan.h"
#include "Pipeline.hpp"

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

		void draw(std::uint32_t vertexCount, std::uint32_t instanceCount, std::uint32_t firstVertex, std::uint32_t firstInstance);

	private:
		VkDevice _device;
		VkCommandPool _commandPool;
		VkCommandBuffer _commandBuffer;
	};
} // namespace Concerto::Graphics::Wrapper

#endif //CONCERTOGRAPHICS_COMMANDBUFFER_HPP
