//
// Created by arthur on 13/09/2022.
//

#ifndef CONCERTOGRAPHICS_RENDEROBJECT_HPP
#define CONCERTOGRAPHICS_RENDEROBJECT_HPP
#include <memory>
#include "vulkan/vulkan.h"
#include "Vulkan/VkMesh.hpp"
#include "Vulkan/Material.hpp"

namespace Concerto::Graphics
{
	struct RenderObject
	{
		explicit RenderObject(std::unique_ptr<VkMesh> mesh, VkPipelineLayout pipelineLayout, VkPipeline pipeline, const glm::mat4& transformMatrix) : mesh(
				std::move(mesh)), material(pipelineLayout, pipeline), transformMatrix(transformMatrix)
		{

		}

		explicit RenderObject(std::unique_ptr<VkMesh> mesh, VkPipelineLayout pipelineLayout, VkPipeline pipeline) : mesh(
			std::move(mesh)), material(pipelineLayout, pipeline), transformMatrix()
		{

		}

		std::unique_ptr<VkMesh> mesh;
		Material material;
		glm::mat4 transformMatrix;
	};
	using RenderObjectPtr = std::shared_ptr<RenderObject>;
}
#endif //CONCERTOGRAPHICS_RENDEROBJECT_HPP
