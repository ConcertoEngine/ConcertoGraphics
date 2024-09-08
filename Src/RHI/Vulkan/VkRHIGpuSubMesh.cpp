//
// Created by arthur on 09/06/22.
//

#include "Concerto/Graphics/Backend/Vulkan/Wrapper/CommandBuffer.hpp"
#include "Concerto/Graphics/RHI/Vulkan/VkRHIGpuSubMesh.hpp"

#include <Concerto/Core/Cast.hpp>

#include "Concerto/Graphics/RHI/Vulkan/VkMaterial.hpp"
#include "Concerto/Graphics/RHI/Vulkan/VkRHIBuffer.hpp"
#include "Concerto/Graphics/RHI/Vulkan/VkRHIDevice.hpp"

namespace Concerto::Graphics::RHI
{
	VkRHIGpuSubMesh::VkRHIGpuSubMesh(RHI::SubMeshPtr meshPtr, RHI::MaterialPtr material, VkRHIDevice& device) :
		GpuSubMesh(std::move(meshPtr), std::move(material))
	{
		_vertexBuffer = device.CreateBuffer(static_cast<BufferUsageFlags>(BufferUsage::Vertex) | static_cast<BufferUsageFlags>(BufferUsage::TransferDst), GetSubMesh()->GetVertices().size() * sizeof(Vertex), true);
	}

	void VkRHIGpuSubMesh::Upload(Vk::CommandBuffer& commandBuffer, Vk::CommandPool& commandPool, Vk::Fence& fence, const Vk::Queue& queue, VkRHIDevice& device)
	{
		Vk::Buffer stagingBuffer(MakeBuffer<Vertex>(device.GetAllocator(), GetSubMesh()->GetVertices().size() * sizeof(Vertex), VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VMA_MEMORY_USAGE_CPU_ONLY, true));
		commandBuffer.ImmediateSubmit(fence, commandPool, queue, [&](Vk::CommandBuffer& cb)
		{
			VkRHIBuffer& vkBuffer = Cast<VkRHIBuffer&>(*_vertexBuffer);
			stagingBuffer.Copy(GetSubMesh()->GetVertices().data(), GetSubMesh()->GetVertices().size() * sizeof(Vertex));
			cb.CopyBuffer(stagingBuffer, vkBuffer, GetSubMesh()->GetVertices().size() * sizeof(Vertex));
		});
	}
}