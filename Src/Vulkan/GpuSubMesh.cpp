//
// Created by arthur on 09/06/22.
//

#include "Concerto/Graphics/Vulkan/Wrapper/CommandBuffer.hpp"
#include "Concerto/Graphics/Vulkan/GpuSubMesh.hpp"
#include "Concerto/Graphics/Vulkan/VkMaterial.hpp"

using namespace Concerto::Graphics;

namespace Concerto::Graphics
{
	GpuSubMesh::GpuSubMesh(SubMeshPtr meshPtr, Allocator& allocator, const VkBufferUsageFlags usage, const VmaMemoryUsage memoryUsage, std::shared_ptr<VkMaterial> material) :
		subMesh(std::move(meshPtr)),
		vertexBuffer(allocator, subMesh->GetVertices().size() * sizeof(Vertex), usage, memoryUsage),
		material(std::move(material))
	{

	}

	void GpuSubMesh::Upload(CommandBuffer& commandBuffer, CommandPool& commandPool, Fence& fence, Queue& queue, Allocator& allocator)
	{
		Buffer stagingBuffer(MakeBuffer<Vertex>(allocator, subMesh->GetVertices().size() * sizeof(Vertex),VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VMA_MEMORY_USAGE_CPU_ONLY));
		commandBuffer.ImmediateSubmit(fence, commandPool, queue, [&](CommandBuffer& cb)
		{
			stagingBuffer.Copy(subMesh->GetVertices().data(), subMesh->GetVertices().size() * sizeof(Vertex));
			cb.CopyBuffer(stagingBuffer, vertexBuffer, subMesh->GetVertices().size() * sizeof(Vertex));
		});
	}

	const Vertices& GpuSubMesh::GetVertices() const
	{
		return subMesh->GetVertices();
	}

	const MaterialPtr& GpuSubMesh::GetMaterial() const
	{
		return subMesh->GetMaterial();
	}

} // Concerto::Graphics::Wrapper