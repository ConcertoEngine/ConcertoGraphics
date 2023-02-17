//
// Created by arthur on 17/02/2023.
//

#include <iostream>
#include <stdexcept>

#include "Mesh.hpp"
#include "tiny_obj_loader.h"

namespace Concerto::Graphics
{
	Mesh::Mesh(std::string file) : _path(std::move(file))
	{
		if (!LoadFromObj(_path))
			throw std::runtime_error("Failed to load mesh from file: " + _path);
	}

	const Vertices& Mesh::GetVertices() const
	{
		return _vertices;
	}

	bool Mesh::LoadFromObj(const std::string& fileName)
	{
		std::string err;
		tinyobj::ObjReaderConfig readerConfig;
		tinyobj::ObjReader reader;

//		readerConfig.mtl_search_path = ;
		if (!reader.ParseFromFile(fileName, readerConfig))
		{
			if (!reader.Error().empty())
			{
				std::cerr << "TinyObjReader error: " << reader.Error() << std::endl;
				return false;
			}
		}
		if (!reader.Warning().empty())
		{
			std::cout << "TinyObjReader: " << reader.Warning();
		}

		if (!err.empty())
		{
			std::cerr << err << std::endl;
		}

		auto& attrib = reader.GetAttrib();
		auto& shapes = reader.GetShapes();
		auto& materials = reader.GetMaterials();

		for (auto& material : materials)
		{
			Material mat{};
			mat.diffuseTexture = material.diffuse_texname;
			mat.diffuseColor.x = material.diffuse[0];
			mat.diffuseColor.y = material.diffuse[1];
			mat.diffuseColor.z = material.diffuse[2];
			mat.metallic = material.metallic;
			mat.specular.x = material.specular[0];
			mat.specular.y = material.specular[1];
			mat.specular.z = material.specular[2];
			mat.roughness = material.roughness;
			mat.anisotropy = material.anisotropy;
			mat.emissiveColor.x = material.emission[0];
			mat.emissiveColor.y = material.emission[1];
			mat.emissiveColor.z = material.emission[2];
			mat.normalTexture = material.normal_texname;
			_materials.push_back(std::move(mat));
		}

		for (auto& shape : shapes)
		{
			// Loop over faces(polygon)
			std::size_t index_offset = 0;
			for (std::size_t f = 0; f < shape.mesh.num_face_vertices.size(); f++)
			{
				//hardcode loading to triangles
				int fv = 3;
				// Loop over vertices in the face.
				for (std::size_t v = 0; v < fv; v++)
				{
					// access to vertex
					tinyobj::index_t idx = shape.mesh.indices[index_offset + v];

					//vertex position
					tinyobj::real_t vx = attrib.vertices[3 * idx.vertex_index + 0];
					tinyobj::real_t vy = attrib.vertices[3 * idx.vertex_index + 1];
					tinyobj::real_t vz = attrib.vertices[3 * idx.vertex_index + 2];
					//vertex normal
					tinyobj::real_t nx = attrib.normals[3 * idx.normal_index + 0];
					tinyobj::real_t ny = attrib.normals[3 * idx.normal_index + 1];
					tinyobj::real_t nz = attrib.normals[3 * idx.normal_index + 2];

					//copy it into our vertex
					Vertex new_vert{};
					new_vert.position.x = vx;
					new_vert.position.y = vy;
					new_vert.position.z = vz;

					new_vert.normal.x = nx;
					new_vert.normal.y = ny;
					new_vert.normal.z = nz;

					//we are setting the vertex color as the vertex normal. This is just for display purposes
					new_vert.color = new_vert.normal;
					//vertex uv
					tinyobj::real_t ux = attrib.texcoords[2 * idx.texcoord_index + 0];
					tinyobj::real_t uy = attrib.texcoords[2 * idx.texcoord_index + 1];

					new_vert.uv.x = ux;
					new_vert.uv.y = 1 - uy;
					_vertices.push_back(new_vert);
				}
				index_offset += fv;
			}
		}
		return true;
	}

	const std::string& Mesh::GetPath() const
	{
		return _path;
	}

	const std::vector<Material>& Mesh::GetMaterials() const
	{
		return _materials;
	}
}