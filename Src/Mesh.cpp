//
// Created by arthur on 17/02/2023.
//

#define TINYOBJLOADER_IMPLEMENTATION

#include <iostream>
#include <filesystem>

#include <Concerto/Core/Logger.hpp>
#include <Concerto/Graphics/thirdParty/tiny_obj_loader.h>

#include "Concerto/Graphics/Mesh.hpp"

namespace Concerto::Graphics
{
	Mesh::Mesh(std::string filePath) : _path(std::move(filePath))
	{
		LoadFromFile(_path);
	}

	Mesh::Mesh(Vertices vertices)
	{
		SubMeshPtr subMesh = std::make_shared<SubMesh>(this);
		subMesh->GetVertices() = std::move(vertices);
		_subMeshes.push_back(subMesh);
	}

	std::vector<SubMeshPtr>& Mesh::GetSubMeshes()
	{
		return _subMeshes;
	}

	bool Mesh::LoadFromFile(const std::string& fileName)
	{
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
		if (!reader.Warning().empty())
		{
			Logger::Error("TinyObjReader: {}", reader.Warning());
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
		std::filesystem::path path = GetPath();
		path = path.parent_path();
		for (auto& material : materials)
		{
			MaterialPtr mat = std::make_shared<MaterialInfo>();
			mat->diffuseTexturePath = material.diffuse_texname.empty() ? "" : (path / material.diffuse_texname).string();
			mat->normalTexturePath = material.normal_texname.empty() ? "" : (path / material.normal_texname).string();
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
					SubMeshPtr subMesh = std::make_shared<SubMesh>(this);
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

	//VkMesh& Mesh::CreateGPUMesh(Device& device)
	//{
	//	//_gpuMesh = std::make_unique<VkMesh>();
	//	//Allocator& allocator = device.GetAllocator();
	//	//UploadContext& uploadContext = device.GetUploadContext();

	//	//for (SubMeshPtr& subMesh : _subMeshes)
	//	//{
	//	//	VkSubMeshPtr vkSubMesh = std::make_shared<VkSubMesh>(subMesh, allocator,
	//	//		VK_BUFFER_USAGE_VERTEX_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT,
	//	//		VMA_MEMORY_USAGE_GPU_ONLY);
	//	//	vkSubMesh->Upload(uploadContext._commandBuffer, uploadContext._commandPool,uploadContext._uploadFence, device.GetQueue(Queue::Type::Graphics), allocator);
	//	//	_gpuMesh->subMeshes.push_back(vkSubMesh);
	//	//	//TODO: Add support for all material properties
	//	//	VkPipelineLayout pipelineLayout = *_meshPipelineLayout->Get();
	//	//	VkPipeline pipeline = *_coloredShaderPipeline->Get();
	//	//	if (!subMesh->GetMaterial()->diffuseTexturePath.empty())
	//	//	{
	//	//		pipelineLayout = *_texturedSetLayout->Get();
	//	//		pipeline = *_texturedPipeline->Get();
	//	//	}
	//	//	if (!subMesh->GetMaterial()->diffuseTexturePath.empty())
	//	//	{
	//	//		std::filesystem::path path = _path;
	//	//		path = path.parent_path() / subMesh->GetMaterial()->diffuseTexturePath;
	//	//		subMesh->GetMaterial()->diffuseTexture = _textureBuilder->BuildTexture(path.string());
	//	//		_materialBuilder->BuildMaterial(*subMesh->GetMaterial(), pipelineLayout, pipeline);
	//	//	}
	//	//	else
	//	//	{
	//	//		_materialBuilder->BuildMaterial(*subMesh->GetMaterial(), pipelineLayout, pipeline);
	//	//	}
	//	}
	//}
}