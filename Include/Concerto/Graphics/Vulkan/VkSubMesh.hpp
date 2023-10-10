//
// Created by arthur on 09/06/22.
//

#ifndef CONCERTOGRAPHICS_MESH_HPP
#define CONCERTOGRAPHICS_MESH_HPP

#include <vector>
#include <string>
#include <memory>

#include "Concerto/Graphics/Defines.hpp"

#include "Concerto/Graphics/Vulkan/Wrapper/Buffer.hpp"
#include "Concerto/Graphics/Vulkan/Wrapper/Allocator.hpp"
#include "Concerto/Graphics/Vulkan/Wrapper/CommandPool.hpp"
#include "Concerto/Graphics/Vulkan/Wrapper/Fence.hpp"
#include "Concerto/Graphics/Vulkan/Wrapper/Queue.hpp"
#include "Concerto/Graphics/SubMesh.hpp"

namespace Concerto::Graphics
{
	struct CONCERTO_GRAPHICS_API VkSubMesh
	{
		VkSubMesh(SubMeshPtr& meshPtr, Allocator& allocator, VkBufferUsageFlags usage,
				VmaMemoryUsage memoryUsage);

		void Upload(CommandBuffer& commandBuffer, CommandPool &commandPool, Fence& fence, Queue& queue,
				Allocator& allocator);

		[[nodiscard]] const Vertices& GetVertices() const;
		[[nodiscard]] const MaterialPtr& GetMaterial() const;
		SubMeshPtr _subMesh;
		Buffer _vertexBuffer;
	};
	using VkSubMeshPtr = std::shared_ptr<VkSubMesh>;
} // Concerto::Graphics::Wrapper

#endif //CONCERTOGRAPHICS_MESH_HPP
