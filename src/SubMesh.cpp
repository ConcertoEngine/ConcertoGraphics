//
// Created by arthur on 18/02/2023.
//

#include "SubMesh.hpp"
#include "Mesh.hpp"

namespace Concerto::Graphics
{
	SubMesh::SubMesh(std::shared_ptr<Mesh>& parent):
		_vertices(), _material(std::make_shared<Material>()), _parent(parent)
	{

	}

	SubMesh::SubMesh(const Vertices& vertices, MaterialPtr& material, std::shared_ptr<Mesh>& parent) :
		_vertices(vertices), _material(material), _parent(parent)
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
	
	std::shared_ptr<Mesh>& SubMesh::GetParent()
	{
		return _parent;
	}
}