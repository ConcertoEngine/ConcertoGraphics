//
// Created by arthur on 12/05/2024.
//

#ifndef CONCERTO_GRAPHICS_RHI_MESH_HPP
#define CONCERTO_GRAPHICS_RHI_MESH_HPP

#include <memory>
#include <unordered_map>
#include <vector>
#include <string>

#include <Concerto/Core/Math/Matrix.hpp>

#include "Concerto/Graphics/RHI/Defines.hpp"
#include "Concerto/Graphics/Vertex.hpp"
#include "Concerto/Graphics/RHI/Material.hpp"

namespace cct::gfx::rhi
{
	class RenderPass;
	class Device;
	class MaterialBuilder;
	class SubMesh;
	class GpuMesh;

	class CONCERTO_GRAPHICS_RHI_BASE_API Mesh
	{
	public:
		explicit Mesh(std::string filePath);
		explicit Mesh(Vertices vertices);
		virtual ~Mesh() = default;

		[[nodiscard]] const std::string& GetPath() const;
		[[nodiscard]] std::vector<std::shared_ptr<rhi::SubMesh>>& GetSubMeshes();
		[[nodiscard]] std::unordered_map<std::string, std::shared_ptr<rhi::MaterialInfo>>& GetMaterials();

		[[nodiscard]] bool LoadFromFile(const std::string& fileName);

		[[nodiscard]] virtual std::unique_ptr<GpuMesh> BuildGpuMesh(rhi::MaterialBuilder& materialBuilder, const rhi::RenderPass& renderPass, rhi::Device& device) = 0;
	private:
		std::string _path;
		std::vector<std::shared_ptr<rhi::SubMesh>> _subMeshes;
		std::unordered_map<std::string, std::shared_ptr<rhi::MaterialInfo>> _materials;
	};
}

#endif //CONCERTO_GRAPHICS_RHI_MESH_HPP