//
// Created by arthur on 09/06/22.
//

#ifndef CONCERTOGRAPHICS_MESH_HPP
#define CONCERTOGRAPHICS_MESH_HPP

#include <vector>
#include <string>
#include "Vertex.hpp"
#include "wrapper/AllocatedBuffer.hpp"
#include "wrapper/Allocator.hpp"

namespace Concerto::Graphics
{
	struct Mesh
	{
		Mesh(Vertices vertices, Wrapper::Allocator& allocator, std::size_t allocSize,
				VkBufferUsageFlags usage, VmaMemoryUsage memoryUsage);

		Mesh(const std::string& file, Wrapper::Allocator& allocator, VkBufferUsageFlags usage, VmaMemoryUsage memoryUsage);

		Vertices _vertices;

		bool loadFromObj(const std::string& fileName, const std::string& materialPath);

		bool _isLoaded;
		Wrapper::AllocatedBuffer _vertexBuffer;
	};
} // Concerto::Graphics::Wrapper

#endif //CONCERTOGRAPHICS_MESH_HPP
