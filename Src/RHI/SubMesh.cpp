//
// Created by arthur on 18/02/2023.
//

#include "Concerto/Graphics/RHI/SubMesh.hpp"
#include "Concerto/Graphics/RHI/Mesh.hpp"
#include "Concerto/Graphics/Backend/Vulkan/Wrapper/ShaderModule.hpp"

namespace Concerto::Graphics::RHI
{
	SubMesh::SubMesh(Mesh* parent) :
		_vertices(),
		_material(std::make_shared<RHI::MaterialInfo>()),
		_parent(parent)
	{

	}

	SubMesh::SubMesh(const Vertices& vertices, std::shared_ptr<RHI::MaterialInfo>& material, Mesh* parent) :
		_vertices(vertices),
		_material(material),
		_parent(parent)
	{

	}

	Vertices& SubMesh::GetVertices()
	{
		return _vertices;
	}

	std::shared_ptr<RHI::MaterialInfo>& SubMesh::GetMaterial()
	{
		return _material;
	}

	Mesh* SubMesh::GetParent()
	{
		return _parent;
	}
}