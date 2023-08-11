//
// Created by arthur on 09/06/22.
//

#define TINYOBJLOADER_IMPLEMENTATION

#include <iostream>
#include <filesystem>
#include <stdexcept>

#include <Concerto/Graphics/thirdParty/tiny_obj_loader.h>

#include "Concerto/Graphics/Vulkan/Wrapper/CommandBuffer.hpp"
#include "Concerto/Graphics/Vulkan/VkSubMesh.hpp"
#include "Concerto/Graphics/Vulkan/Utils.hpp"

using namespace Concerto::Graphics;

namespace Concerto::Graphics
{
	VkSubMesh::VkSubMesh(SubMeshPtr& meshPtr, Allocator& allocator, VkBufferUsageFlags usage,
		VmaMemoryUsage memoryUsage) : _subMesh(meshPtr), _vertexBuffer(allocator,
		_subMesh->GetVertices().size() * sizeof(Vertex),
		usage,
		memoryUsage)
	{

	}

	void VkSubMesh::Upload(CommandBuffer& commandBuffer, CommandPool& commandPool, Fence& fence, Queue& queue,
		Allocator& allocator)
	{
		Buffer stagingBuffer(MakeBuffer<Vertex>(allocator, _subMesh->GetVertices().size() * sizeof(Vertex),
			VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
			VMA_MEMORY_USAGE_CPU_ONLY));
		commandBuffer.ImmediateSubmit(fence, commandPool, queue, [&](CommandBuffer& cb)
		{
		  MapAndCopy(allocator,
			  stagingBuffer,
			  _subMesh->GetVertices().data(),
			  _subMesh->GetVertices().size() * sizeof(Vertex));
		  cb.CopyBuffer(stagingBuffer, _vertexBuffer, _subMesh->GetVertices().size() * sizeof(Vertex));
		});
	}

	const Vertices& VkSubMesh::GetVertices() const
	{
		return _subMesh->GetVertices();
	}
	const MaterialPtr& VkSubMesh::GetMaterial() const
	{
		return _subMesh->GetMaterial();
	}

} // Concerto::Graphics::Wrapper