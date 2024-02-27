//
// Created by arthur on 09/06/22.
//

#ifndef CONCERTO_GRAPHICS_MESH_HPP
#define CONCERTO_GRAPHICS_MESH_HPP

#include <memory>

#include "Concerto/Graphics/Defines.hpp"

#include "Concerto/Graphics/Vulkan/Wrapper/Buffer.hpp"
#include "Concerto/Graphics/SubMesh.hpp"

namespace Concerto::Graphics
{
	class Allocator;
	class CommandPool;
	struct VkMaterial;

	struct CONCERTO_GRAPHICS_API GpuSubMesh
	{
		GpuSubMesh(SubMeshPtr meshPtr, Allocator& allocator, VkBufferUsageFlags usage, VmaMemoryUsage memoryUsage, std::shared_ptr<VkMaterial> material);

		void Upload(CommandBuffer& commandBuffer, CommandPool &commandPool, Fence& fence, Queue& queue,	Allocator& allocator);

		[[nodiscard]] const Vertices& GetVertices() const;
		[[nodiscard]] const MaterialPtr& GetMaterial() const;
		SubMeshPtr subMesh;
		Buffer vertexBuffer;
		std::shared_ptr<VkMaterial> material;
	};
	using GpuSubMeshPtr = std::shared_ptr<GpuSubMesh>;
} // Concerto::Graphics::Wrapper

#endif //CONCERTO_GRAPHICS_MESH_HPP
