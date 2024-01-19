//
// Created by arthur on 18/02/2023.
//

#ifndef CONCERTO_GRAPHICS_INCLUDE_SUBMESH_HPP_
#define CONCERTO_GRAPHICS_INCLUDE_SUBMESH_HPP_

#include <memory>

#include "Concerto/Graphics/Defines.hpp"
#include "Concerto/Graphics/Vulkan/Vertex.hpp"
#include "Concerto/Graphics/Material.hpp"

namespace Concerto::Graphics
{
	class Mesh;
	class CONCERTO_GRAPHICS_API SubMesh
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
#endif //CONCERTO_GRAPHICS_INCLUDE_SUBMESH_HPP_
