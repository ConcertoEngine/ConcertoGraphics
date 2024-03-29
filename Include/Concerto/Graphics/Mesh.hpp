//
// Created by arthur on 17/02/2023.
//

#ifndef CONCERTO_GRAPHICS_INCLUDE_MESH_HPP_
#define CONCERTO_GRAPHICS_INCLUDE_MESH_HPP_

#include <unordered_map>
#include <memory>

#include "MaterialBuilder.hpp"
#include "Concerto/Graphics/Defines.hpp"
#include "Concerto/Graphics/Material.hpp"
#include "Concerto/Graphics/SubMesh.hpp"

namespace Concerto::Graphics
{
	class Device;
	class GpuMesh;
	class MaterialInfo;

	class CONCERTO_GRAPHICS_API Mesh
	{
	public:
		explicit Mesh(std::string filePath);
		
		explicit Mesh(Vertices vertices);

		[[nodiscard]] const std::string& GetPath() const;

		std::vector<SubMeshPtr>& GetSubMeshes();

		std::unordered_map<std::string, std::shared_ptr<MaterialInfo>>& GetMaterials();

		bool LoadFromFile(const std::string& fileName);
		std::shared_ptr<GpuMesh> BuildGpuMesh(MaterialBuilder& materialBuilder, const RenderPass& renderPass, Device& device, UploadContext& uploadContext);

	private:
		std::string _path;
		std::vector<SubMeshPtr> _subMeshes;
		std::unordered_map<std::string, std::shared_ptr<MaterialInfo>> _materials;
	};
	using MeshPtr = std::shared_ptr<Mesh>;
}

#endif //CONCERTO_GRAPHICS_INCLUDE_MESH_HPP_
