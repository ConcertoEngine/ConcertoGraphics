//
// Created by arthur on 03/09/2024.
//

#ifndef CONCERTO_GRAPHICS_RHI_COMMANDBUFFER_HPP
#define CONCERTO_GRAPHICS_RHI_COMMANDBUFFER_HPP

namespace Concerto::Graphics::RHI
{
	class CONCERTO_GRAPHICS_API CommandBuffer
	{
	public:
		CommandBuffer() = default;
		virtual ~CommandBuffer() = default;
	};
}

#endif //CONCERTO_GRAPHICS_RHI_COMMANDBUFFER_HPP