//
// Created by arthur on 09/06/22.
//

#ifndef CONCERTO_GRAPHICS_RHI_GPUSUBMESH_HPP
#define CONCERTO_GRAPHICS_RHI_GPUSUBMESH_HPP

#include <memory>

#include "Concerto/Graphics/Defines.hpp"
#include "Concerto/Graphics/RHI/SubMesh.hpp"
#include "Concerto/Graphics/RHI/Material.hpp"

namespace Concerto::Graphics::RHI
{
	class CONCERTO_GRAPHICS_API GpuSubMesh
	{
	public:
		GpuSubMesh(RHI::SubMeshPtr meshPtr, RHI::MaterialPtr material);

		[[nodiscard]] const Vertices& GetVertices() const;
		[[nodiscard]] const RHI::MaterialPtr& GetMaterial() const;
		[[nodiscard]] const RHI::SubMeshPtr& GetSubMesh() const;
	private:
		RHI::SubMeshPtr _subMesh;
		RHI::MaterialPtr _material;
	};
	using GpuSubMeshPtr = std::shared_ptr<GpuSubMesh>;
} // Concerto::Graphics::Vk

#endif //CONCERTO_GRAPHICS_RHI_GPUSUBMESH_HPP
