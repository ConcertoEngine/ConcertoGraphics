//
// Created by arthur on 09/06/22.
//

#include "Concerto/Graphics/RHI/GpuSubMesh.hpp"

namespace cct::gfx::rhi
{
	GpuSubMesh::GpuSubMesh(rhi::SubMeshPtr meshPtr, rhi::MaterialPtr material) :
		m_subMesh(std::move(meshPtr)),
		m_material(std::move(material))
	{

	}

	const Vertices& GpuSubMesh::GetVertices() const
	{
		return m_subMesh->GetVertices();
	}

	const rhi::MaterialPtr& GpuSubMesh::GetMaterial() const
	{
		return m_material;
	}

	const rhi::SubMeshPtr& GpuSubMesh::GetSubMesh() const
	{
		return m_subMesh;
	}

	const rhi::Buffer& GpuSubMesh::GetVertexBuffer() const
	{
		CCT_ASSERT(m_vertexBuffer, "ConcertoGraphics: invalid vertex buffer");
		return *m_vertexBuffer;
	}
} // cct::gfx::RHI