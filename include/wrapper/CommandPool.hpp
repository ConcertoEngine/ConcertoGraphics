//
// Created by arthur on 14/06/22.
//

#ifndef CONCERTOGRAPHICS_COMMANDPOOL_HPP
#define CONCERTOGRAPHICS_COMMANDPOOL_HPP
#include "vulkan/vulkan.h"
#include <cstdint>
namespace Concerto::Graphics::Wrapper
{
	class CommandPool
	{
	public:
		CommandPool(VkDevice device, std::uint32_t queueFamily);

		CommandPool(CommandPool&&) = default;

		CommandPool(const CommandPool&) = delete;

		CommandPool& operator=(CommandPool&&) = default;

		CommandPool& operator=(const CommandPool&) = delete;

		~CommandPool();
		VkCommandPool get() const;
	private:
		VkDevice _device;
		VkCommandPool _commandPool;
		std::uint32_t _queueFamily;
	};
}

#endif //CONCERTOGRAPHICS_COMMANDPOOL_HPP
