//
// Created by arthur on 03/09/2024.
//

#ifndef CONCERTO_GRAPHICS_RHI_COMMANDPOOL_HPP
#define CONCERTO_GRAPHICS_RHI_COMMANDPOOL_HPP
#include <memory>

namespace Concerto::Graphics::RHI
{
	class CommandBuffer;

	enum class CommandBufferUasge
	{
		Primary,
		Secondary
	};

	class CONCERTO_GRAPHICS_API CommandPool
	{
	public:
		CommandPool() = default;
		virtual ~CommandPool() = default;
		virtual std::unique_ptr<CommandBuffer> AllocateCommandBuffer(CommandBufferUasge usage) = 0;
	};
}

#endif //CONCERTO_GRAPHICS_RHI_COMMANDPOOL_HPP