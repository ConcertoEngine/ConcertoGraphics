//
// Created by arthur on 18/02/2023.
//

#ifndef CONCERTOGRAPHICS_INCLUDE_SUBMESH_HPP_
#define CONCERTOGRAPHICS_INCLUDE_SUBMESH_HPP_

#include <memory>

#include <Concerto/Core/Types.hpp>
#include "Vulkan/Vertex.hpp"
#include "Material.hpp"

namespace Concerto::Graphics
{
	class Mesh;
	class CONCERTO_PUBLIC_API SubMesh
	{
	 public:
		explicit SubMesh(Mesh* parent);
		SubMesh(const Vertices& vertices, MaterialPtr& material, Mesh* parent);
		[[nodiscard]] Vertices& GetVertices();
		[[nodiscard]] MaterialPtr& GetMaterial();
		[[nodiscard]] Mesh* GetParent();
	 private:
		Vertices _vertices;
		MaterialPtr _material;
		Mesh* _parent;
	};
	using SubMeshPtr = std::shared_ptr<SubMesh>;
}
#endif //CONCERTOGRAPHICS_INCLUDE_SUBMESH_HPP_
