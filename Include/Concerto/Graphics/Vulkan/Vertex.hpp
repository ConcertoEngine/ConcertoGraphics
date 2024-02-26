//
// Created by arthur on 09/06/22.
//

#ifndef CONCERTO_GRAPHICS_VERTEX_HPP
#define CONCERTO_GRAPHICS_VERTEX_HPP

#include <vector>

#include <vulkan/vulkan.hpp>
#include <Concerto/Core/Math/Vector.hpp>

#include "Concerto/Graphics/Defines.hpp"

namespace Concerto::Graphics
{

	struct VertexInputDescription
	{

		std::vector<VkVertexInputBindingDescription> bindings;
		std::vector<VkVertexInputAttributeDescription> attributes;

		VkPipelineVertexInputStateCreateFlags flags = 0;
	};

	struct CONCERTO_GRAPHICS_API Vertex
	{
		Vector3f position;
		Vector3f normal;
		Vector3f color;
		Vector2f uv;

		static VertexInputDescription GetVertexDescription();
	};
	using Vertices = std::vector<Vertex>;

} // Concerto

#endif //CONCERTO_GRAPHICS_VERTEX_HPP
