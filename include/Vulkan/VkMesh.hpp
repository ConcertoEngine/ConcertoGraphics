//
// Created by arthur on 18/02/2023.
//

#ifndef CONCERTOGRAPHICS_INCLUDE_VULKAN_VKMESH_HPP_
#define CONCERTOGRAPHICS_INCLUDE_VULKAN_VKMESH_HPP_

#include <memory>
#include <vector>
#include <glm/mat4x4.hpp>
#include "VkSubMesh.hpp"

namespace Concerto::Graphics
{
	struct VkMesh
	{
		std::vector<VkSubMeshPtr> subMeshes;
		glm::mat4 transformMatrix;
	};
	using VkMeshPtr = std::shared_ptr<VkMesh>;
}
#endif //CONCERTOGRAPHICS_INCLUDE_VULKAN_VKMESH_HPP_
