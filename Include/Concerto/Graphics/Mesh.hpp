//
// Created by arthur on 17/02/2023.
//

#ifndef CONCERTOGRAPHICS_INCLUDE_MESH_HPP_
#define CONCERTOGRAPHICS_INCLUDE_MESH_HPP_

#include <unordered_map>
#include <memory>

#include "Concerto/Graphics/Defines.hpp"

#include "Concerto/Graphics/Vulkan/Vertex.hpp"
#include "Concerto/Graphics/Material.hpp"
#include "Concerto/Graphics/SubMesh.hpp"

namespace Concerto::Graphics
{
	class Device;
	class VkMesh;

	class CONCERTO_GRAPHICS_API Mesh
	{
	 public:
		Mesh(std::string filePath);
		
		Mesh(Vertices vertices);
		
		const std::string& GetPath() const;
		std::vector<SubMeshPtr>& GetSubMeshes();
		std::unordered_map<std::string, MaterialPtr>& GetMaterials();

		bool LoadFromFile(const std::string& fileName);

		//VkMesh& CreateGPUMesh(Device& device);
	 private:
		std::string _path;
		std::vector<SubMeshPtr> _subMeshes;
		std::unordered_map<std::string, MaterialPtr> _materials;
		//std::unique_ptr<VkMesh> _gpuMesh;
	};
	using MeshPtr = std::shared_ptr<Mesh>;
}

#endif //CONCERTOGRAPHICS_INCLUDE_MESH_HPP_
