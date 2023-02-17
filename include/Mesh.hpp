//
// Created by arthur on 17/02/2023.
//

#ifndef CONCERTOGRAPHICS_INCLUDE_MESH_HPP_
#define CONCERTOGRAPHICS_INCLUDE_MESH_HPP_

#include <memory>
#include "Vulkan/Vertex.hpp"
#include "Material.hpp"

namespace Concerto::Graphics
{
	class Mesh : public std::enable_shared_from_this<Mesh>
	{
	 public:
		Mesh(std::string file);
		const Vertices& GetVertices() const;
		const std::string& GetPath() const;
		const std::vector<Material>& GetMaterials() const;
	 private:
		bool LoadFromObj(const std::string& fileName);
		Vertices _vertices;
		std::vector<Material> _materials;
		std::string _path;
	};
	using MeshPtr = std::shared_ptr<Mesh>;
}

#endif //CONCERTOGRAPHICS_INCLUDE_MESH_HPP_
