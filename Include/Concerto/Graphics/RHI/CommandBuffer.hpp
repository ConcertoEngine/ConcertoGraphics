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

		virtual void Begin() = 0;
		virtual void End() = 0;
		virtual void Submit() = 0;
		virtual void Reset() = 0;
	};
}

#endif //CONCERTO_GRAPHICS_RHI_COMMANDBUFFER_HPP