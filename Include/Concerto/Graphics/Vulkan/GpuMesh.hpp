//
// Created by arthur on 18/02/2023.
//

#ifndef CONCERTO_GRAPHICS_INCLUDE_VULKAN_VKMESH_HPP_
#define CONCERTO_GRAPHICS_INCLUDE_VULKAN_VKMESH_HPP_

#include <memory>
#include <vector>

#include <Concerto/Core/Math/Matrix.hpp>

#include "Concerto/Graphics/Vulkan/GpuSubMesh.hpp"

namespace Concerto::Graphics
{
	class GpuMesh
	{
	public:
		std::vector<GpuSubMeshPtr> subMeshes;
		Matrix4f transformMatrix;
	};
	using GpuMeshPtr = std::shared_ptr<GpuMesh>;
}
#endif //CONCERTO_GRAPHICS_INCLUDE_VULKAN_VKMESH_HPP_
