//
// Created by arthur on 09/06/22.
//

#ifndef CONCERTOGRAPHICS_MESH_HPP
#define CONCERTOGRAPHICS_MESH_HPP

#include <vector>
#include <string>

#include "Vulkan/Wrapper/Buffer.hpp"
#include "Vulkan/Wrapper/Allocator.hpp"
#include "Vulkan/Wrapper/CommandPool.hpp"
#include "Vulkan/Wrapper/Fence.hpp"
#include "Vulkan/Wrapper/Queue.hpp"
#include "Mesh.hpp"

namespace Concerto::Graphics
{
	struct VkMesh
	{
		VkMesh(MeshPtr& meshPtr, Wrapper::Allocator& allocator, VkBufferUsageFlags usage,
				VmaMemoryUsage memoryUsage);

		void Upload(Wrapper::CommandBuffer& commandBuffer, Wrapper::CommandPool &commandPool, Wrapper::Fence& fence, Wrapper::Queue& queue,
				Wrapper::Allocator& allocator);

		const Vertices& GetVertices() const;
		MeshPtr _mesh;
		Wrapper::Buffer _vertexBuffer;
	};
} // Concerto::Graphics::Wrapper

#endif //CONCERTOGRAPHICS_MESH_HPP
