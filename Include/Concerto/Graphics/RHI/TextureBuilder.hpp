//
// Created by arthur on 07/08/2024.
//

#ifndef CONCERTO_GRAPHICS_RHI_TEXTUREBUILDER_HPP
#define CONCERTO_GRAPHICS_RHI_TEXTUREBUILDER_HPP

#include <memory>

#include "Concerto/Graphics/RHI/Defines.hpp"
#include "Concerto/Graphics/RHI/Texture.hpp"

namespace cct::gfx::rhi
{
	class CONCERTO_GRAPHICS_RHI_BASE_API TextureBuilder
	{
	public:
		virtual ~TextureBuilder() = default;

		virtual std::shared_ptr<Texture> BuildTexture(const std::string& path) = 0;
	};
}

#endif //CONCERTO_GRAPHICS_RHI_TEXTUREBUILDER_HPP