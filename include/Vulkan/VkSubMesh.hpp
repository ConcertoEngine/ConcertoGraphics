//
// Created by arthur on 09/06/22.
//

#ifndef CONCERTOGRAPHICS_MESH_HPP
#define CONCERTOGRAPHICS_MESH_HPP

#include <vector>
#include <string>
#include <memory>

#include "Vulkan/Wrapper/Buffer.hpp"
#include "Vulkan/Wrapper/Allocator.hpp"
#include "Vulkan/Wrapper/CommandPool.hpp"
#include "Vulkan/Wrapper/Fence.hpp"
#include "Vulkan/Wrapper/Queue.hpp"
#include "SubMesh.hpp"

namespace Concerto::Graphics
{
	struct VkSubMesh
	{
		VkSubMesh(SubMeshPtr& meshPtr, Wrapper::Allocator& allocator, VkBufferUsageFlags usage,
				VmaMemoryUsage memoryUsage);

		void Upload(Wrapper::CommandBuffer& commandBuffer, Wrapper::CommandPool &commandPool, Wrapper::Fence& fence, Wrapper::Queue& queue,
				Wrapper::Allocator& allocator);

		[[nodiscard]] const Vertices& GetVertices() const;
		[[nodiscard]] const MaterialPtr& GetMaterial() const;
		SubMeshPtr _subMesh;
		Wrapper::Buffer _vertexBuffer;
	};
	using VkSubMeshPtr = std::shared_ptr<VkSubMesh>;
} // Concerto::Graphics::Wrapper

#endif //CONCERTOGRAPHICS_MESH_HPP
