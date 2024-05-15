//
// Created by arthur on 12/05/2024.
//

#ifndef CONCERTO_GRAPHICS_RHI_SWAPCHAIN_HPP
#define CONCERTO_GRAPHICS_RHI_SWAPCHAIN_HPP

#include "Concerto/Graphics/Defines.hpp"

namespace Concerto::Graphics::RHI
{
	class CONCERTO_GRAPHICS_API SwapChain
	{
	public:
		SwapChain() = default;
		virtual ~SwapChain() = default;
		virtual void Present() = 0;
	};
}

#endif //CONCERTO_GRAPHICS_RHI_SWAPCHAIN_HPP