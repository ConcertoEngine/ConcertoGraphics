//
// Created by arthur on 12/05/2024.
//

#ifndef CONCERTO_GRAPHICS_RHI_TEXTURE_HPP
#define CONCERTO_GRAPHICS_RHI_TEXTURE_HPP

#include "Concerto/Graphics/Defines.hpp"
#include "Concerto/Graphics/RHI/Enums.hpp"

namespace Concerto::Graphics::RHI
{
	class CONCERTO_GRAPHICS_API Texture
	{
	public:
		Texture() = default;
		virtual ~Texture() = default;
	};
}


#endif //CONCERTO_GRAPHICS_RHI_TEXTURE_HPP