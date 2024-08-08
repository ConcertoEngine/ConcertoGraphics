//
// Created by arthur on 12/05/2024.
//

#ifndef CONCERTO_GRAPHICS_RHI_SWAPCHAIN_HPP
#define CONCERTO_GRAPHICS_RHI_SWAPCHAIN_HPP

#include <Concerto/Core/Math/Vector.hpp>

#include "Concerto/Graphics/RHI/RenderPass.hpp"
#include "Concerto/Graphics/Defines.hpp"
#include "Concerto/Graphics/RHI/Enums.hpp"

namespace Concerto::Graphics::RHI
{
	class RenderPass;

	class CONCERTO_GRAPHICS_API SwapChain
	{
	public:
		inline SwapChain(PixelFormat pixelFormat, PixelFormat depthPixelFormat);
		virtual ~SwapChain() = default;
		virtual void Present() = 0;

		virtual RenderPass& GetRenderPass() = 0;
		virtual Vector2u GetExtent() = 0;
		inline PixelFormat GetPixelFormat() const;
		inline PixelFormat GetDepthPixelFormat() const;

	private:
		PixelFormat _pixelFormat;
		PixelFormat _depthPixelFormat;
	};
}

#include "Concerto/Graphics/RHI/Vulkan/VkRHISwapChain.inl"

#endif //CONCERTO_GRAPHICS_RHI_SWAPCHAIN_HPP