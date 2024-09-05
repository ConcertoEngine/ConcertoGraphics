//
// Created by arthur on 05/09/2024.
//

#ifndef CONCERTO_GRAPHICS_RHI_FRAME_HPP
#define CONCERTO_GRAPHICS_RHI_FRAME_HPP

namespace Concerto::Graphics::RHI
{
	class CONCERTO_GRAPHICS_API Frame
	{
	public:
		virtual ~Frame() = default;
		virtual void Present() = 0;
	};
}

#endif //CONCERTO_GRAPHICS_RHI_FRAME_HPP