//
// Created by arthur on 01/09/2025.
//

#ifndef CONCERTO_GRAPHICS_RHI_DX12_DX12RHITEXTURE_HPP
#define CONCERTO_GRAPHICS_RHI_DX12_DX12RHITEXTURE_HPP

#include "Concerto/Graphics/RHI/Texture.hpp"

namespace cct::gfx::rhi
{
	class CONCERTO_GRAPHICS_RHI_BASE_API Dx12RHITexture : public rhi::Texture
	{
	public:
		Dx12RHITexture() = default;
	};
}

#endif //CONCERTO_GRAPHICS_RHI_DX12_DX12RHITEXTURE_HPP