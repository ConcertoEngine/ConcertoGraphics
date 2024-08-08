//
// Created by arthur on 09/06/22.
//

#include "Concerto/Graphics/RHI/GpuSubMesh.hpp"

namespace Concerto::Graphics::RHI
{
	GpuSubMesh::GpuSubMesh(RHI::SubMeshPtr meshPtr, RHI::MaterialPtr material) :
		_subMesh(std::move(meshPtr)),
		_material(std::move(material))
	{

	}

	const Vertices& GpuSubMesh::GetVertices() const
	{
		return _subMesh->GetVertices();
	}

	const RHI::MaterialPtr& GpuSubMesh::GetMaterial() const
	{
		return _subMesh->GetMaterial();
	}

	const RHI::SubMeshPtr& GpuSubMesh::GetSubMesh() const
	{
		return _subMesh;
	}
} // Concerto::Graphics::RHI