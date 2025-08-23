//
// Created by arthur on 09/06/22.
//

#ifndef CONCERTO_GRAPHICS_RHI_GPUSUBMESH_HPP
#define CONCERTO_GRAPHICS_RHI_GPUSUBMESH_HPP

#include <memory>

#include "Buffer.hpp"
#include "Concerto/Graphics/RHI/Defines.hpp"
#include "Concerto/Graphics/RHI/SubMesh.hpp"
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
		rhi::SubMeshPtr _subMesh;
		rhi::MaterialPtr _material;
	protected:
		std::unique_ptr<rhi::Buffer> _vertexBuffer;
	};
	using GpuSubMeshPtr = std::shared_ptr<GpuSubMesh>;
} // cct::gfx::vk

#endif //CONCERTO_GRAPHICS_RHI_GPUSUBMESH_HPP
