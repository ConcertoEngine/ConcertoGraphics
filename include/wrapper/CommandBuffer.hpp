//
// Created by arthur on 14/06/22.
//

#ifndef CONCERTOGRAPHICS_COMMANDBUFFER_HPP
#define CONCERTOGRAPHICS_COMMANDBUFFER_HPP

#include "vulkan/vulkan.h"

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

	private:
		VkDevice _device;
		VkCommandPool _commandPool;
		VkCommandBuffer _commandBuffer;
	};
}

#endif //CONCERTOGRAPHICS_COMMANDBUFFER_HPP
