//
// Created by arthur on 09/06/22.
//

#ifndef CONCERTO_GRAPHICS_RHI_GPUSUBMESH_HPP
#define CONCERTO_GRAPHICS_RHI_GPUSUBMESH_HPP

#include <memory>

#include "Concerto/Graphics/RHI/Buffer.hpp"
#include "Concerto/Graphics/RHI/Defines.hpp"
#include "Concerto/Graphics/RHI/SubMesh/SubMesh.hpp"
#include "Concerto/Graphics/RHI/Material.hpp"
#include "Concerto/Graphics/RHI/Buffer.hpp"

namespace cct::gfx::rhi
{
	class CONCERTO_GRAPHICS_RHI_BASE_API GpuSubMesh
	{
	public:
		GpuSubMesh(rhi::SubMeshPtr meshPtr, rhi::MaterialPtr material);
		virtual ~GpuSubMesh() = default;

		[[nodiscard]] const Vertices& GetVertices() const;
		[[nodiscard]] const rhi::MaterialPtr& GetMaterial() const;
		[[nodiscard]] const rhi::SubMeshPtr& GetSubMesh() const;
		const rhi::Buffer& GetVertexBuffer() const;

	private:
		rhi::SubMeshPtr m_subMesh;
		rhi::MaterialPtr m_material;
	protected:
		std::unique_ptr<rhi::Buffer> m_vertexBuffer;
	};
	using GpuSubMeshPtr = std::shared_ptr<GpuSubMesh>;
} // cct::gfx::vk

#endif //CONCERTO_GRAPHICS_RHI_GPUSUBMESH_HPP
