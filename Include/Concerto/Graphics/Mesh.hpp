//
// Created by arthur on 17/02/2023.
//

#ifndef CONCERTOGRAPHICS_INCLUDE_MESH_HPP_
#define CONCERTOGRAPHICS_INCLUDE_MESH_HPP_

#include <unordered_map>

#include <Concerto/Core/Types.hpp>

#include "Concerto/Graphics/Vulkan/Vertex.hpp"
#include "Concerto/Graphics/Material.hpp"
#include "Concerto/Graphics/SubMesh.hpp"

namespace Concerto::Graphics
{
	class CONCERTO_PUBLIC_API Mesh
	{
	 public:
		Mesh(std::string filePath);
		Mesh(Vertices vertices);
		std::vector<SubMeshPtr>& GetSubMeshes();
		const std::string& GetPath() const;
		bool LoadFromFile(const std::string& fileName);
		std::unordered_map<std::string, MaterialPtr>& GetMaterials();
	 private:
		std::string _path;
		std::vector<SubMeshPtr> _subMeshes;
		std::unordered_map<std::string, MaterialPtr> _materials;
	};
	using MeshPtr = std::shared_ptr<Mesh>;
}

#endif //CONCERTOGRAPHICS_INCLUDE_MESH_HPP_
