//
// Created by arthur on 17/02/2023.
//

#include <iostream>
#include <stdexcept>

#include "Mesh.hpp"
#include "tiny_obj_loader.h"

namespace Concerto::Graphics
{
	std::vector<SubMeshPtr>& Mesh::GetSubMeshes()
	{
		return _subMeshes;
	}

	bool Mesh::LoadFromFile(const std::string& fileName)
	{
		_path = fileName;
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
			return false;
		}

		auto& attrib = reader.GetAttrib();
		auto& shapes = reader.GetShapes();
		auto& materials = reader.GetMaterials();

		int i = 0;
		for (auto& material : materials)
		{
			MaterialPtr mat = std::make_shared<Material>();
			mat->diffuseTexturePath = material.diffuse_texname;
			mat->diffuseColor.x = material.diffuse[0];
			mat->diffuseColor.y = material.diffuse[1];
			mat->diffuseColor.z = material.diffuse[2];
			mat->metallic = material.metallic;
			mat->specular.x = material.specular[0];
			mat->specular.y = material.specular[1];
			mat->specular.z = material.specular[2];
			mat->roughness = material.roughness;
			mat->anisotropy = material.anisotropy;
			mat->emissiveColor.x = material.emission[0];
			mat->emissiveColor.y = material.emission[1];
			mat->emissiveColor.z = material.emission[2];
			mat->normalTexturePath = material.normal_texname;
			mat->name = material.name;
			_materials[material.name] = std::move(mat);
			++i;
		}

		int currentSubMeshIndex = -1;
		for (const auto& shape : shapes)
		{
			std::size_t index_offset = 0;
			const int fv = 3;
			for (std::size_t f = 0; f < shape.mesh.num_face_vertices.size(); f++)
			{
				const int matId = shape.mesh.material_ids[f];
				if (currentSubMeshIndex == -1
					|| _subMeshes[currentSubMeshIndex]->GetMaterial()->name != materials[matId].name)
				{
					MeshPtr self = shared_from_this();
					SubMeshPtr subMesh = std::make_shared<SubMesh>(self);
					subMesh->GetMaterial() = _materials[materials[matId].name];
					_subMeshes.push_back(subMesh);
					currentSubMeshIndex++;
				}

				SubMeshPtr& currentSubMesh = _subMeshes[currentSubMeshIndex];

				for (std::size_t v = 0; v < fv; v++)
				{
					tinyobj::index_t idx = shape.mesh.indices[index_offset + v];

					//vertex position
					tinyobj::real_t vx = attrib.vertices[3 * idx.vertex_index + 0];
					tinyobj::real_t vy = attrib.vertices[3 * idx.vertex_index + 1];
					tinyobj::real_t vz = attrib.vertices[3 * idx.vertex_index + 2];
					//vertex normal
					tinyobj::real_t nx = attrib.normals[3 * idx.normal_index + 0];
					tinyobj::real_t ny = attrib.normals[3 * idx.normal_index + 1];
					tinyobj::real_t nz = attrib.normals[3 * idx.normal_index + 2];

					tinyobj::real_t ux = attrib.texcoords[2 * idx.texcoord_index + 0];
					tinyobj::real_t uy = attrib.texcoords[2 * idx.texcoord_index + 1];

					currentSubMesh->GetVertices().emplace_back(Vertex{{ vx, vy, vz },
																	  { nx, ny, nz },
																	  { nx, ny, nz },
																	  { ux, 1 - uy }});
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

	std::unordered_map<std::string, MaterialPtr>& Mesh::GetMaterials()
	{
		return _materials;
	}
}