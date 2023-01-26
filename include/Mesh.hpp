//
// Created by arthur on 09/06/22.
//

#ifndef CONCERTOGRAPHICS_MESH_HPP
#define CONCERTOGRAPHICS_MESH_HPP

#include <vector>
#include <string>
#include "Vertex.hpp"
#include "wrapper/Buffer.hpp"
#include "wrapper/Allocator.hpp"
#include "wrapper/CommandPool.hpp"
#include "wrapper/Fence.hpp"
#include "wrapper/Queue.hpp"

namespace Concerto::Graphics
{
	struct Mesh
	{
		Mesh(Vertices vertices, Wrapper::Allocator& allocator, std::size_t allocSize,
				VkBufferUsageFlags usage, VmaMemoryUsage memoryUsage);

		Mesh(const std::string& file, Wrapper::Allocator& allocator, VkBufferUsageFlags usage,
				VmaMemoryUsage memoryUsage);


		bool loadFromObj(const std::string& fileName, const std::string& materialPath);

		void Upload(Wrapper::CommandBuffer& commandBuffer, Wrapper::CommandPool &commandPool, Wrapper::Fence& fence, Wrapper::Queue& queue,
				Wrapper::Allocator& allocator);

		Vertices _vertices;
		bool _isLoaded;
		Wrapper::Buffer _vertexBuffer;
	};
} // Concerto::Graphics::Wrapper

#endif //CONCERTOGRAPHICS_MESH_HPP
