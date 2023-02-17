//
// Created by arthur on 09/06/22.
//

#ifndef CONCERTOGRAPHICS_MESH_HPP
#define CONCERTOGRAPHICS_MESH_HPP

#include <vector>
#include <string>
#include "Vulkan/Vertex.hpp"
#include "Vulkan/wrapper/Buffer.hpp"
#include "Vulkan/wrapper/Allocator.hpp"
#include "Vulkan/wrapper/CommandPool.hpp"
#include "Vulkan/Wrapper/Fence.hpp"
#include "Vulkan/Wrapper/Queue.hpp"

namespace Concerto::Graphics
{
	struct VkMesh
	{
		VkMesh(Vertices vertices, Wrapper::Allocator& allocator, std::size_t allocSize,
				VkBufferUsageFlags usage, VmaMemoryUsage memoryUsage);

		VkMesh(const std::string& file, Wrapper::Allocator& allocator, VkBufferUsageFlags usage,
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
