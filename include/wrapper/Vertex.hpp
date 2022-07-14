//
// Created by arthur on 09/06/22.
//

#ifndef CONCERTOGRAPHICS_VERTEX_HPP
#define CONCERTOGRAPHICS_VERTEX_HPP

#include <glm/vec3.hpp>
#include <glm/glm.hpp>
#include <vulkan/vulkan.hpp>

namespace Concerto::Graphics::Wrapper
{

	struct VertexInputDescription
	{

		std::vector<VkVertexInputBindingDescription> bindings;
		std::vector<VkVertexInputAttributeDescription> attributes;

		VkPipelineVertexInputStateCreateFlags flags = 0;
	};

	struct Vertex
	{
		glm::vec3 position;
		glm::vec3 normal;
		glm::vec3 color;
		glm::vec2 uv;

		static VertexInputDescription getVertexDescription();
	};

} // Concerto

#endif //CONCERTOGRAPHICS_VERTEX_HPP
