//
// Created by arthur on 03/09/2024.
//

#ifndef CONCERTO_GRAPHICS_RHI_COMMANDPOOL_HPP
#define CONCERTO_GRAPHICS_RHI_COMMANDPOOL_HPP
#include <memory>

#include "Concerto/Graphics/RHI/CommandBuffer.hpp"

namespace cct::gfx::rhi
{

	enum class CommandBufferUasge
	{
		Primary,
		Secondary
	};

	class CONCERTO_GRAPHICS_RHI_BASE_API CommandPool
	{
	public:
		virtual ~CommandPool() = default;
		virtual std::unique_ptr<CommandBuffer> AllocateCommandBuffer(CommandBufferUasge usage) = 0;
	};
}

#endif //CONCERTO_GRAPHICS_RHI_COMMANDPOOL_HPP