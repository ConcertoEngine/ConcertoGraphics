//
// Created by arthur on 09/06/22.
//

#define TINYOBJLOADER_IMPLEMENTATION

#include <iostream>
#include <filesystem>
#include <stdexcept>

#include "Vulkan/Wrapper/CommandBuffer.hpp"
#include "Vulkan/VkMesh.hpp"
#include "tiny_obj_loader.h"
#include "Vulkan/Utils.hpp"

using namespace Concerto::Graphics::Wrapper;

namespace Concerto::Graphics
{
	VkMesh::VkMesh(MeshPtr& meshPtr, Allocator& allocator, VkBufferUsageFlags usage,
		VmaMemoryUsage memoryUsage) : _mesh(meshPtr), _vertexBuffer(allocator,
		_mesh->GetVertices().size() * sizeof(Vertex),
		usage,
		memoryUsage)
	{

	}

	void VkMesh::Upload(CommandBuffer& commandBuffer, CommandPool& commandPool, Fence& fence, Queue& queue,
		Allocator& allocator)
	{
		Buffer stagingBuffer(MakeAllocatedBuffer<Vertex>(allocator, _mesh->GetVertices().size() * sizeof(Vertex),
			VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
			VMA_MEMORY_USAGE_CPU_ONLY));
		commandBuffer.ImmediateSubmit(fence, commandPool, queue, [&](CommandBuffer& cb)
		{
		  MapAndCopy(allocator,
			  stagingBuffer,
			  _mesh->GetVertices().data(),
			  _mesh->GetVertices().size() * sizeof(Vertex));
		  cb.CopyBuffer(stagingBuffer, _vertexBuffer, _mesh->GetVertices().size() * sizeof(Vertex));
		});
	}
	const Vertices& VkMesh::GetVertices() const
	{
		return _mesh->GetVertices();
	}

} // Concerto::Graphics::Wrapper