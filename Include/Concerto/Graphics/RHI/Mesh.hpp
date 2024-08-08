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

#include "Concerto/Graphics/Defines.hpp"
#include "Concerto/Graphics/Vertex.hpp"
#include "Concerto/Graphics/RHI/Material.hpp"

namespace Concerto::Graphics::RHI
{
	class RenderPass;
	class Device;
	class MaterialBuilder;
	class SubMesh;
	class GpuMesh;

	class CONCERTO_GRAPHICS_API Mesh
	{
	public:
		explicit Mesh(std::string filePath);
		explicit Mesh(Vertices vertices);
		virtual ~Mesh() = default;

		[[nodiscard]] const std::string& GetPath() const;
		[[nodiscard]] std::vector<std::shared_ptr<RHI::SubMesh>>& GetSubMeshes();
		[[nodiscard]] std::unordered_map<std::string, std::shared_ptr<RHI::MaterialInfo>>& GetMaterials();

		[[nodiscard]] bool LoadFromFile(const std::string& fileName);

		[[nodiscard]] virtual std::shared_ptr<GpuMesh> BuildGpuMesh(RHI::MaterialBuilder& materialBuilder, const RHI::RenderPass& renderPass, RHI::Device& device) = 0;
	private:
		std::string _path;
		std::vector<std::shared_ptr<RHI::SubMesh>> _subMeshes;
		std::unordered_map<std::string, std::shared_ptr<RHI::MaterialInfo>> _materials;
	};
}

#endif //CONCERTO_GRAPHICS_RHI_MESH_HPP