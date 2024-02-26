//
// Created by arthur on 18/02/2023.
//

#ifndef CONCERTO_GRAPHICS_INCLUDE_VULKAN_VKMESH_HPP_
#define CONCERTO_GRAPHICS_INCLUDE_VULKAN_VKMESH_HPP_

#include <memory>
#include <vector>

#include <Concerto/Core/Math/Matrix.hpp>

#include "Concerto/Graphics/Vulkan/VkSubMesh.hpp"

namespace Concerto::Graphics
{
	class VkMesh
	{
	public:
		std::vector<VkSubMeshPtr> subMeshes;
		Matrix4f transformMatrix;
	};
	using VkMeshPtr = std::shared_ptr<VkMesh>;
}
#endif //CONCERTO_GRAPHICS_INCLUDE_VULKAN_VKMESH_HPP_
