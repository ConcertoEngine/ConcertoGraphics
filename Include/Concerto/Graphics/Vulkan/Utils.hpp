//
// Created by arthur on 14/09/2022.
//

#ifndef CONCERTO_GRAPHICS_UTILS_HPP
#define CONCERTO_GRAPHICS_UTILS_HPP

#include <glm/vec4.hpp>
#include <glm/mat4x4.hpp>

namespace Concerto::Graphics
{
	class MeshPushConstants
	{
		glm::vec4 _data = {};
		glm::mat4 _renderMatrix = {};
	};

	inline std::size_t PadUniformBuffer(std::size_t size, std::size_t minUniformBufferOffsetAlignment)
	{
		if (size > 0)
			return (size + minUniformBufferOffsetAlignment - 1) & ~(minUniformBufferOffsetAlignment - 1);
		return size;
	}
}

#endif //CONCERTO_GRAPHICS_UTILS_HPP
