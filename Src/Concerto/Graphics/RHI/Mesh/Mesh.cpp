//
// Created by arthur on 17/02/2023.
//

#define TINYOBJLOADER_IMPLEMENTATION

#include <filesystem>
#include <tiny_obj_loader.h>
#include <Concerto/Core/Logger.hpp>
#include <Concerto/Core/Cast.hpp>

#include "Concerto/Graphics/RHI/Mesh/Mesh.hpp"
#include "Concerto/Graphics/RHI/Material.hpp"
#include "Concerto/Graphics/RHI/SubMesh/SubMesh.hpp"

namespace cct::gfx::rhi
{
	Mesh::Mesh(std::string filePath) : m_path(std::move(filePath))
	{
		bool loaded = LoadFromFile(m_path);
		CCT_ASSERT(loaded, "LoadFromFile failed");
	}

	Mesh::Mesh(Vertices vertices)
	{
		rhi::SubMeshPtr subMesh = std::make_shared<rhi::SubMesh>(this);
		subMesh->GetVertices() = std::move(vertices);
		m_subMeshes.push_back(subMesh);
	}

	std::vector<rhi::SubMeshPtr>& Mesh::GetSubMeshes()
	{
		return m_subMeshes;
	}

	bool Mesh::LoadFromFile(const std::string& fileName)
	{
		CCT_GFX_AUTO_PROFILER_SCOPE();

		std::string err;
		tinyobj::ObjReaderConfig readerConfig;
		tinyobj::ObjReader reader;

		if (!reader.ParseFromFile(fileName, readerConfig))
		{
			if (!reader.Error().empty())
			{
				Logger::Error("TinyObjReader: {}", reader.Error());
				return false;
			}
		}

		if (!err.empty())
		{
			Logger::Error("Error while loading Obj file '{}': {}", fileName, err);
			return false;
		}

		auto& attrib = reader.GetAttrib();
		auto& shapes = reader.GetShapes();
		auto& materials = reader.GetMaterials();

		int i = 0;
		std::filesystem::path path = GetPath();
		path = path.parent_path();
		for (auto& material : materials)
		{
			std::shared_ptr<rhi::MaterialInfo> mat = std::make_shared<rhi::MaterialInfo>();
			std::string diffuse_texname = material.diffuse_texname;
			std::string normal_texname = material.normal_texname;
			#ifdef CCT_PLATFORM_POSIX
			auto cleanPath = [](std::string& p)
			{
				std::replace(p.begin(), p.end(), '\\', '/');
			};
			cleanPath(diffuse_texname);
			cleanPath(normal_texname);
			#endif // CCT_PLATFORM_POSIX

			mat->diffuseTexturePath = diffuse_texname.empty() ? "" : (path / diffuse_texname).string();
			mat->normalTexturePath = normal_texname.empty() ? "" : (path / normal_texname).string();
			//mat->diffuseColor.x = material.diffuse[0];
			//mat->diffuseColor.y = material.diffuse[1];
			//mat->diffuseColor.z = material.diffuse[2];
			mat->metallic = material.metallic;
			//mat->specular.x = material.specular[0];
			//mat->specular.y = material.specular[1];
			//mat->specular.z = material.specular[2];
			mat->roughness = material.roughness;
			mat->anisotropy = material.anisotropy;
			//mat->emissiveColor.x = material.emission[0];
			//mat->emissiveColor.y = material.emission[1];
			//mat->emissiveColor.z = material.emission[2];
			mat->name = material.name;
			m_materials[material.name] = std::move(mat);
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
					|| m_subMeshes[currentSubMeshIndex]->GetMaterial()->name != materials[matId].name)
				{
					SubMeshPtr subMesh = std::make_shared<SubMesh>(this);
					subMesh->GetMaterial() = m_materials[materials[matId].name];
					m_subMeshes.push_back(subMesh);
					currentSubMeshIndex++;
				}

				SubMeshPtr& currentSubMesh = m_subMeshes[currentSubMeshIndex];

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

					currentSubMesh->GetVertices().emplace_back(Vertex{ Vector3f{ vx, vy, vz },
																	  Vector3f{ nx, ny, nz },
																	  Vector3f{ nx, ny, nz },
																	  Vector2f{ ux, 1 - uy } });
				}
				index_offset += fv;
			}
		}
		return true;
	}

	const std::string& Mesh::GetPath() const
	{
		return m_path;
	}

	std::unordered_map<std::string, std::shared_ptr<rhi::MaterialInfo>>& Mesh::GetMaterials()
	{
		return m_materials;
	}
}
