//
// Created by arthur on 09/06/22.
//

#ifndef CONCERTOGRAPHICS_VERTEX_HPP
#define CONCERTOGRAPHICS_VERTEX_HPP

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <vulkan/vulkan.hpp>
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
		glm::vec3 position;
		glm::vec3 normal;
		glm::vec3 color;
		glm::vec2 uv;

		static VertexInputDescription GetVertexDescription();
	};
	using Vertices = std::vector<Vertex>;

} // Concerto

#endif //CONCERTOGRAPHICS_VERTEX_HPP
