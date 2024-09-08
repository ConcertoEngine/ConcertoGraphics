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
		return _material;
	}

	const RHI::SubMeshPtr& GpuSubMesh::GetSubMesh() const
	{
		return _subMesh;
	}

	const RHI::Buffer& GpuSubMesh::GetVertexBuffer() const
	{
		CONCERTO_ASSERT(_vertexBuffer, "ConcertoGraphics: invalid vertex buffer");
		return *_vertexBuffer;
	}
} // Concerto::Graphics::RHI