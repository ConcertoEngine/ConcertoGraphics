//
// Created by arthur on 09/06/22.
//

#ifndef CONCERTO_GRAPHICS_RHI_VULKAN_VKRHIGPUSUBMESH_HPP
#define CONCERTO_GRAPHICS_RHI_VULKAN_VKRHIGPUSUBMESH_HPP

#include "Concerto/Graphics/RHI/Defines.hpp"
#include "Concerto/Graphics/RHI/GpuSubMesh.hpp"
#include "Concerto/Graphics/Backend/Vulkan/Wrapper/Buffer.hpp"

namespace Concerto::Graphics::Vk
{
	class CommandPool;
	class Fence;
}

namespace Concerto::Graphics::RHI
{
	class VkRHIDevice;
	class CONCERTO_GRAPHICS_RHI_BASE_API VkRHIGpuSubMesh : public RHI::GpuSubMesh
	{
	public:
		VkRHIGpuSubMesh(RHI::SubMeshPtr meshPtr, RHI::MaterialPtr material, VkRHIDevice& device);

		void Upload(Vk::CommandBuffer& commandBuffer, Vk::CommandPool& commandPool, Vk::Fence& fence, const Vk::Queue& queue, VkRHIDevice& device);
	};
}

#endif //CONCERTO_GRAPHICS_RHI_VULKAN_VKRHIGPUSUBMESH_HPP