//
// Created by arthur on 14/09/2022.
//

#ifndef CONCERTO_GRAPHICS_UTILS_HPP
#define CONCERTO_GRAPHICS_UTILS_HPP

#include <Concerto/Core/Math/Vector.hpp>
#include <Concerto/Core/Math/Matrix.hpp>

namespace Concerto::Graphics::Vk
{
	class MeshPushConstants
	{
		Vector4f _data = {};
		Matrix4f _renderMatrix = {};
	};
}

#endif //CONCERTO_GRAPHICS_UTILS_HPP
