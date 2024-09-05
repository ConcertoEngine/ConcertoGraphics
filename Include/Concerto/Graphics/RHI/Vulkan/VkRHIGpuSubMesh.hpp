//
// Created by arthur on 09/06/22.
//

#ifndef CONCERTO_GRAPHICS_RHI_VULKAN_VKRHIGPUSUBMESH_HPP
#define CONCERTO_GRAPHICS_RHI_VULKAN_VKRHIGPUSUBMESH_HPP

#include <vk_mem_alloc.h>
#include "Concerto/Graphics/RHI/GpuSubMesh.hpp"
#include "Concerto/Graphics/Backend/Vulkan/Wrapper/Buffer.hpp"

namespace Concerto::Graphics::Vk
{
	class CommandPool;
	class Fence;
}

namespace Concerto::Graphics::RHI
{
	class CONCERTO_GRAPHICS_VKRHI_API VkRHIGpuSubMesh : public RHI::GpuSubMesh
	{
	public:
		VkRHIGpuSubMesh(RHI::SubMeshPtr meshPtr, RHI::MaterialPtr material, Vk::Allocator& allocator, VkBufferUsageFlags usage, VmaMemoryUsage memoryUsage);

		void Upload(Vk::CommandBuffer& commandBuffer, Vk::CommandPool& commandPool, Vk::Fence& fence, const Vk::Queue& queue, Vk::Allocator& allocator);
	private:
		Vk::Buffer vertexBuffer;
	};
}

#endif //CONCERTO_GRAPHICS_RHI_VULKAN_VKRHIGPUSUBMESH_HPP