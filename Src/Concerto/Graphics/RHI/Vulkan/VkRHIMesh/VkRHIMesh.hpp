//
// Created by arthur on 17/02/2023.
//

#ifndef CONCERTO_GRAPHICS_RHI_VKRHIMESH_HPP
#define CONCERTO_GRAPHICS_RHI_VKRHIMESH_HPP

#include <unordered_map>
#include <memory>

#include "Concerto/Graphics/RHI/MaterialBuilder.hpp"
#include "Concerto/Graphics/Defines.hpp"
#include "Concerto/Graphics/RHI/Material.hpp"
#include "Concerto/Graphics/RHI/Mesh/Mesh.hpp"
#include "Concerto/Graphics/RHI/SubMesh/SubMesh.hpp"

namespace cct::gfx::vk
{
	class GpuMesh;
}

namespace cct::gfx::rhi
{
	class CONCERTO_GRAPHICS_RHI_BASE_API VkRHIMesh : public rhi::Mesh
	{
	public:
		explicit VkRHIMesh(std::string filePath);
		explicit VkRHIMesh(Vertices vertices);

		std::unique_ptr<rhi::GpuMesh> BuildGpuMesh(rhi::MaterialBuilder& materialBuilder, const rhi::RenderPass& renderPass, rhi::Device& device) override;
	};
	using MeshPtr = std::shared_ptr<VkRHIMesh>;
}

#endif //CONCERTO_GRAPHICS_RHI_VKRHIMESH_HPP
