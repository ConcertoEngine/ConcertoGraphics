//
// Created by arthur on 14/09/2022.
//

#ifndef CONCERTO_GRAPHICS_UTILS_HPP
#define CONCERTO_GRAPHICS_UTILS_HPP

#include <Concerto/Core/Math/Vector.hpp>
#include <Concerto/Core/Math/Matrix.hpp>

namespace cct::gfx::vk
{
	class MeshPushConstants
	{
		Vector4f m_data = {};
		Matrix4f m_renderMatrix = {};
	};
}

#endif //CONCERTO_GRAPHICS_UTILS_HPP
