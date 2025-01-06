//
// Created by arthur on 09/06/22.
//

#ifndef CONCERTO_GRAPHICS_RHI_VULKAN_VKRHIGPUSUBMESH_HPP
#define CONCERTO_GRAPHICS_RHI_VULKAN_VKRHIGPUSUBMESH_HPP

#include "Concerto/Graphics/RHI/Defines.hpp"
#include "Concerto/Graphics/RHI/GpuSubMesh.hpp"
#include "Concerto/Graphics/Backend/Vulkan/Wrapper/Buffer.hpp"

namespace cct::gfx::vk
{
	class CommandPool;
	class Fence;
}

namespace cct::gfx::rhi
{
	class VkRHIDevice;
	class CONCERTO_GRAPHICS_RHI_BASE_API VkRHIGpuSubMesh : public rhi::GpuSubMesh
	{
	public:
		VkRHIGpuSubMesh(rhi::SubMeshPtr meshPtr, rhi::MaterialPtr material, VkRHIDevice& device);

		void Upload(vk::CommandBuffer& commandBuffer, vk::CommandPool& commandPool, vk::Fence& fence, const vk::Queue& queue, VkRHIDevice& device);
	};
}

#endif //CONCERTO_GRAPHICS_RHI_VULKAN_VKRHIGPUSUBMESH_HPP