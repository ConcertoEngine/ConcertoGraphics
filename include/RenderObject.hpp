//
// Created by arthur on 13/09/2022.
//

#ifndef CONCERTOGRAPHICS_RENDEROBJECT_HPP
#define CONCERTOGRAPHICS_RENDEROBJECT_HPP
#include <memory>
#include "vulkan/vulkan.h"
#include "Mesh.hpp"
#include "Material.hpp"

namespace Concerto::Graphics
{
	struct RenderObject
	{
		explicit RenderObject(std::unique_ptr<Mesh> mesh, VkPipelineLayout pipelineLayout, VkPipeline pipeline, const glm::mat4& transformMatrix) : mesh(
				std::move(mesh)), material(pipelineLayout, pipeline), transformMatrix(transformMatrix)
		{

		}

		std::unique_ptr<Mesh> mesh;
		Material material;
		glm::mat4 transformMatrix;
	};
}
#endif //CONCERTOGRAPHICS_RENDEROBJECT_HPP
