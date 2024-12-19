//
// Created by arthur on 09/06/22.
//

#include "Concerto/Graphics/RHI/GpuSubMesh.hpp"

namespace cct::gfx::rhi
{
	GpuSubMesh::GpuSubMesh(rhi::SubMeshPtr meshPtr, rhi::MaterialPtr material) :
		_subMesh(std::move(meshPtr)),
		_material(std::move(material))
	{

	}

	const Vertices& GpuSubMesh::GetVertices() const
	{
		return _subMesh->GetVertices();
	}

	const rhi::MaterialPtr& GpuSubMesh::GetMaterial() const
	{
		return _material;
	}

	const rhi::SubMeshPtr& GpuSubMesh::GetSubMesh() const
	{
		return _subMesh;
	}

	const rhi::Buffer& GpuSubMesh::GetVertexBuffer() const
	{
		CCT_ASSERT(_vertexBuffer, "ConcertoGraphics: invalid vertex buffer");
		return *_vertexBuffer;
	}
} // cct::gfx::RHI