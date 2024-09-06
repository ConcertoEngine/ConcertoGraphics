//
// Created by arthur on 09/06/22.
//

#include "Concerto/Graphics/Backend/Vulkan/Wrapper/CommandBuffer.hpp"
#include "Concerto/Graphics/RHI/Vulkan/VkRHIGpuSubMesh.hpp"
#include "Concerto/Graphics/RHI/Vulkan/VkMaterial.hpp"

namespace Concerto::Graphics::RHI
{
	VkRHIGpuSubMesh::VkRHIGpuSubMesh(RHI::SubMeshPtr meshPtr, RHI::MaterialPtr material, Vk::Allocator& allocator, VkBufferUsageFlags usage, VmaMemoryUsage memoryUsage) :
		GpuSubMesh(std::move(meshPtr), std::move(material)),
		vertexBuffer(allocator, GetSubMesh()->GetVertices().size() * sizeof(Vertex), usage, memoryUsage, true)
	{

	}

	void VkRHIGpuSubMesh::Upload(Vk::CommandBuffer& commandBuffer, Vk::CommandPool& commandPool, Vk::Fence& fence, const Vk::Queue& queue, Vk::Allocator& allocator)
	{
		Vk::Buffer stagingBuffer(MakeBuffer<Vertex>(allocator, GetSubMesh()->GetVertices().size() * sizeof(Vertex), VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VMA_MEMORY_USAGE_CPU_ONLY, true));
		commandBuffer.ImmediateSubmit(fence, commandPool, queue, [&](Vk::CommandBuffer& cb)
		{
			stagingBuffer.Copy(GetSubMesh()->GetVertices().data(), GetSubMesh()->GetVertices().size() * sizeof(Vertex));
			cb.CopyBuffer(stagingBuffer, vertexBuffer, GetSubMesh()->GetVertices().size() * sizeof(Vertex));
		});
	}
}