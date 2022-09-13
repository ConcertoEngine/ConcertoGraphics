//
// Created by arthur on 09/06/22.
//

#define TINYOBJLOADER_IMPLEMENTATION
#include <iostream>
#include <filesystem>
#include <stdexcept>

#include "tiny_obj_loader.h"
#include "Mesh.hpp"

using namespace Concerto::Graphics::Wrapper;

namespace Concerto::Graphics
{
	Mesh::Mesh(Vertices vertices, Allocator& allocator, std::size_t allocSize, VkBufferUsageFlags usage,
			VmaMemoryUsage memoryUsage) : _isLoaded(!vertices.empty()),
										  _vertices(std::move(vertices)),
										  _vertexBuffer(allocator, allocSize, usage, memoryUsage)
	{
		if (!_isLoaded)
			throw std::runtime_error("Empty vertices");
		void* data;
		vmaMapMemory(allocator._allocator, _vertexBuffer._allocation, &data);

		std::memcpy(data, _vertices.data(), allocSize);

		vmaUnmapMemory(allocator._allocator, _vertexBuffer._allocation);
	}

	Mesh::Mesh(const std::string& file, Allocator& allocator, VkBufferUsageFlags usage,
			VmaMemoryUsage memoryUsage) : _isLoaded(
			loadFromObj(file, std::filesystem::path(file).parent_path().string())),
										  _vertexBuffer(allocator, _vertices.size() * sizeof(Vertex), usage,
												  memoryUsage)
	{
		if (!_isLoaded)
			throw std::runtime_error("Empty vertices");
		void* data;
		vmaMapMemory(allocator._allocator, _vertexBuffer._allocation, &data);

		std::memcpy(data, _vertices.data(), _vertices.size() * sizeof(Vertex));

		vmaUnmapMemory(allocator._allocator, _vertexBuffer._allocation);
	}

	bool Mesh::loadFromObj(const std::string& fileName, const std::string& materialPath)
	{
		std::string err;
		tinyobj::ObjReaderConfig readerConfig;
		tinyobj::ObjReader reader;

		readerConfig.mtl_search_path = materialPath;
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

		for (auto& shape: shapes)
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


} // Concerto::Graphics::Wrapper