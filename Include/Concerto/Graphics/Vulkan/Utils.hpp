//
// Created by arthur on 14/09/2022.
//

#ifndef CONCERTO_GRAPHICS_UTILS_HPP
#define CONCERTO_GRAPHICS_UTILS_HPP

#include <Concerto/Core/Math/Vector.hpp>
#include <Concerto/Core/Math/Matrix.hpp>

namespace Concerto::Graphics
{
	class MeshPushConstants
	{
		Vector4f _data = {};
		Matrix4f _renderMatrix = {};
	};

	inline std::size_t PadUniformBuffer(std::size_t size, std::size_t minUniformBufferOffsetAlignment)
	{
		if (size > 0)
			return (size + minUniformBufferOffsetAlignment - 1) & ~(minUniformBufferOffsetAlignment - 1);
		return size;
	}
}

#endif //CONCERTO_GRAPHICS_UTILS_HPP
