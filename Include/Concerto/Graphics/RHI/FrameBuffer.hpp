//
// Created by arthur on 12/05/2024.
//

#ifndef CONCERTO_GRAPHICS_RHI_FRAMEBUFFER_HPP
#define CONCERTO_GRAPHICS_RHI_FRAMEBUFFER_HPP

#include "Concerto/Graphics/RHI/Defines.hpp"
#include "Concerto/Graphics/RHI/Enums.hpp"

namespace Concerto::Graphics::RHI
{
	class Texture;
	class RenderPass;
	class Device;

	class CONCERTO_GRAPHICS_RHI_BASE_API FrameBuffer
	{
	public:
		virtual ~FrameBuffer() = default;
	};
}


#endif //CONCERTO_GRAPHICS_RHI_FRAMEBUFFER_HPP