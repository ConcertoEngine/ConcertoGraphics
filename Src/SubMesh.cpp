//
// Created by arthur on 18/02/2023.
//

#include "Concerto/Graphics/SubMesh.hpp"
#include "Concerto/Graphics/Mesh.hpp"
#include "Concerto/Graphics/Backend/Vulkan/Wrapper/ShaderModule.hpp"

namespace Concerto::Graphics
{
	SubMesh::SubMesh(Mesh* parent) :
		_vertices(),
		_material(std::make_shared<MaterialInfo>()),
		_parent(parent)
	{

	}

	SubMesh::SubMesh(const Vk::Vertices& vertices, std::shared_ptr<MaterialInfo>& material, Mesh* parent) :
		_vertices(vertices),
		_material(material),
		_parent(parent)
	{

	}

	Vk::Vertices& SubMesh::GetVertices()
	{
		return _vertices;
	}

	std::shared_ptr<MaterialInfo>& SubMesh::GetMaterial()
	{
		return _material;
	}
	
	Mesh* SubMesh::GetParent()
	{
		return _parent;
	}
}