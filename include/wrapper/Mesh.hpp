//
// Created by arthur on 09/06/22.
//

#ifndef CONCERTOGRAPHICS_MESH_HPP
#define CONCERTOGRAPHICS_MESH_HPP

#include <vector>
#include <string>
#include "AllocatedBuffer.hpp"
#include "Vertex.hpp"
#include "glm/glm.hpp"

namespace Concerto::Graphics::Wrapper
{
	struct Mesh
	{
		Mesh(Vertices vertices, Allocator& allocator, std::size_t allocSize,
				VkBufferUsageFlags usage, VmaMemoryUsage memoryUsage);

		Mesh(const std::string& file, Allocator& allocator, VkBufferUsageFlags usage, VmaMemoryUsage memoryUsage);

		Vertices _vertices;

		bool loadFromObj(const std::string& fileName, const std::string& materialPath);

		bool _isLoaded;
		AllocatedBuffer _vertexBuffer;
	private:
	};
} // Concerto::Graphics::Wrapper

#endif //CONCERTOGRAPHICS_MESH_HPP
