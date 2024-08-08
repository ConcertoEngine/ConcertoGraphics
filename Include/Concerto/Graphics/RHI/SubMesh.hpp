//
// Created by arthur on 18/02/2023.
//

#ifndef CONCERTO_GRAPHICS_RHI_SUBMESH_HPP
#define CONCERTO_GRAPHICS_RHI_SUBMESH_HPP

#include <memory>

#include "Concerto/Graphics/Defines.hpp"
#include "Concerto/Graphics/Backend/Vulkan/Vertex.hpp"
#include "Concerto/Graphics/RHI/Material.hpp"
#include "Concerto/Graphics/Vertex.hpp"
#include "Concerto/Graphics/RHI/Mesh.hpp"

namespace Concerto::Graphics::RHI
{
	class CONCERTO_GRAPHICS_API SubMesh
	{
	 public:
		explicit SubMesh(Mesh* parent);
		SubMesh(const Vertices& vertices, std::shared_ptr<RHI::MaterialInfo>& material, Mesh* parent);
		[[nodiscard]] Vertices& GetVertices();
		[[nodiscard]] std::shared_ptr<RHI::MaterialInfo>& GetMaterial();
		[[nodiscard]] Mesh* GetParent();
	 private:
		Vertices _vertices;
		std::shared_ptr<RHI::MaterialInfo> _material;
		Mesh* _parent;
	};
	using SubMeshPtr = std::shared_ptr<SubMesh>;
}
#endif //CONCERTO_GRAPHICS_RHI_SUBMESH_HPP
