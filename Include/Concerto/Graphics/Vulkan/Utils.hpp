//
// Created by arthur on 14/09/2022.
//

#ifndef CONCERTOGRAPHICS_UTILS_HPP
#define CONCERTOGRAPHICS_UTILS_HPP

#include <vector>
#include <functional>

#include <glm/vec4.hpp>
#include <glm/mat4x4.hpp>

#include "Concerto/Graphics/Vulkan/Wrapper/Allocator.hpp"
#include "Concerto/Graphics/Vulkan/Wrapper/Buffer.hpp"
#include "Concerto/Graphics/Vulkan/Wrapper/Device.hpp"

namespace Concerto::Graphics
{
	struct MeshPushConstants
	{
		glm::vec4 data;
		glm::mat4 render_matrix;
	};

	inline std::size_t PadUniformBuffer(std::size_t size, std::size_t minUniformBufferOffsetAlignment)
	{
		if (size > 0)
			return (size + minUniformBufferOffsetAlignment - 1) & ~(minUniformBufferOffsetAlignment - 1);
		return size;
	}
}

#endif //CONCERTOGRAPHICS_UTILS_HPP
