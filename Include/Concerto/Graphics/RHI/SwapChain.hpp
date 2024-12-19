//
// Created by arthur on 12/05/2024.
//

#ifndef CONCERTO_GRAPHICS_RHI_SWAPCHAIN_HPP
#define CONCERTO_GRAPHICS_RHI_SWAPCHAIN_HPP

#include <Concerto/Core/Math/Vector.hpp>

#include "Concerto/Graphics/RHI/Frame.hpp"
#include "Concerto/Graphics/RHI/RenderPass.hpp"
#include "Concerto/Graphics/RHI/Defines.hpp"
#include "Concerto/Graphics/RHI/Enums.hpp"

namespace cct::gfx::rhi
{
	class RenderPass;

	class CONCERTO_GRAPHICS_RHI_BASE_API SwapChain
	{
	public:
		inline SwapChain(PixelFormat pixelFormat, PixelFormat depthPixelFormat);
		virtual ~SwapChain() = default;

		virtual RenderPass& GetRenderPass() = 0;
		virtual Vector2u GetExtent() const = 0;
		virtual UInt32 GetImageCount() const = 0;
		virtual rhi::Frame& AcquireFrame() = 0;

		[[nodiscard]] inline PixelFormat GetPixelFormat() const;
		[[nodiscard]] inline PixelFormat GetDepthPixelFormat() const;

	private:
		PixelFormat _pixelFormat;
		PixelFormat _depthPixelFormat;
	};
}

#include "Concerto/Graphics/RHI/Vulkan/VkRHISwapChain.inl"

#endif //CONCERTO_GRAPHICS_RHI_SWAPCHAIN_HPP