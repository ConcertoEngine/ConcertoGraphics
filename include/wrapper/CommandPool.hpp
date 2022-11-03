//
// Created by arthur on 14/06/22.
//

#ifndef CONCERTOGRAPHICS_COMMANDPOOL_HPP
#define CONCERTOGRAPHICS_COMMANDPOOL_HPP

#include <cstdint>
#include "vulkan/vulkan.h"
#include "Object.hpp"

namespace Concerto::Graphics::Wrapper
{
	class Device;
	class CommandPool : public Object<VkCommandPool>
	{
	public:
		CommandPool(Device& device, std::uint32_t queueFamily);

		CommandPool(CommandPool&&) = default;

		CommandPool(const CommandPool&) = delete;

		CommandPool& operator=(CommandPool&&) = default;

		CommandPool& operator=(const CommandPool&) = delete;

		~CommandPool();

		void reset();

	private:
		std::uint32_t _queueFamily;
	};
}

#endif //CONCERTOGRAPHICS_COMMANDPOOL_HPP
