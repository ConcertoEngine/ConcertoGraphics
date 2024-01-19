//
// Created by arthur on 09/06/22.
//

#ifndef CONCERTO_GRAPHICS_MESH_HPP
#define CONCERTO_GRAPHICS_MESH_HPP

#include <memory>

#include "Concerto/Graphics/Defines.hpp"

#include "Concerto/Graphics/Vulkan/Wrapper/Buffer.hpp"
#include "Concerto/Graphics/Vulkan/Wrapper/Allocator.hpp"
#include "Concerto/Graphics/Vulkan/Wrapper/CommandPool.hpp"
#include "Concerto/Graphics/Vulkan/VkMaterial.hpp"
#include "Concerto/Graphics/SubMesh.hpp"

namespace Concerto::Graphics
{
	struct CONCERTO_GRAPHICS_API VkSubMesh
	{
		VkSubMesh(SubMeshPtr meshPtr, Allocator& allocator, VkBufferUsageFlags usage, VmaMemoryUsage memoryUsage, VkMaterialPtr material);

		void Upload(CommandBuffer& commandBuffer, CommandPool &commandPool, Fence& fence, Queue& queue,	Allocator& allocator);

		[[nodiscard]] const Vertices& GetVertices() const;
		[[nodiscard]] const MaterialPtr& GetMaterial() const;
		SubMeshPtr _subMesh;
		Buffer _vertexBuffer;
		VkMaterialPtr _material;
	};
	using VkSubMeshPtr = std::shared_ptr<VkSubMesh>;
} // Concerto::Graphics::Wrapper

#endif //CONCERTO_GRAPHICS_MESH_HPP
