//
// Created by arthur on 03/09/2024.
//

#ifndef CONCERTO_GRAPHICS_RHI_COMMANDBUFFER_HPP
#define CONCERTO_GRAPHICS_RHI_COMMANDBUFFER_HPP

#include <Concerto/Core/Math/Vector.hpp>

#include "Concerto/Graphics/Utils.hpp"

#include "Concerto/Graphics/RHI/Defines.hpp"

namespace Concerto::Graphics::RHI
{
	class RenderPass;
	class SwapChain;

	class CONCERTO_GRAPHICS_RHI_BASE_API CommandBuffer
	{
	public:
		virtual ~CommandBuffer() = default;

		virtual void Begin() = 0;
		virtual void End() = 0;
		virtual void Submit() = 0;
		virtual void Reset() = 0;
		virtual void SetViewport(const Viewport& viewport) = 0;
		virtual void SetScissor(const Rect2D& scissor) = 0;
		virtual void BeginRenderPass(const RHI::RenderPass& renderPass, const RHI::SwapChain& swapChain, const Vector3f& clearColor) = 0;
		virtual void EndRenderPass() = 0;
	};
}

#endif //CONCERTO_GRAPHICS_RHI_COMMANDBUFFER_HPP