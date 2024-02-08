//
// Created by arthur on 18/02/2023.
//

#include "Concerto/Graphics/SubMesh.hpp"
#include "Concerto/Graphics/Mesh.hpp"

namespace Concerto::Graphics
{
	SubMesh::SubMesh(Mesh* parent) :
		_vertices(),
		_material(std::make_shared<MaterialInfo>()),
		_parent(parent)
	{

	}

	SubMesh::SubMesh(const Vertices& vertices, MaterialPtr& material, Mesh* parent) :
		_vertices(vertices),
		_material(material),
		_parent(parent)
	{

	}

	Vertices& SubMesh::GetVertices()
	{
		return _vertices;
	}

	MaterialPtr& SubMesh::GetMaterial()
	{
		return _material;
	}
	
	Mesh* SubMesh::GetParent()
	{
		return _parent;
	}
}