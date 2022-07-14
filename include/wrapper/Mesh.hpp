//
// Created by arthur on 09/06/22.
//

#ifndef CONCERTOGRAPHICS_MESH_HPP
#define CONCERTOGRAPHICS_MESH_HPP

#include <vector>
#include <string>
#include "AllocatedBuffer.hpp"
#include "Vertex.hpp"

namespace Concerto::Graphics::Wrapper
{
	struct Mesh {
		Mesh(Vertices vertices, Allocator& allocator, std::size_t allocSize,
				VkBufferUsageFlags usage, VmaMemoryUsage memoryUsage);
		Vertices _vertices;
		bool LoadFromObj(const std::string &fileName, const std::string &materialPath);
		AllocatedBuffer _vertexBuffer;
	};

} // Concerto::Graphics::Wrapper

#endif //CONCERTOGRAPHICS_MESH_HPP
