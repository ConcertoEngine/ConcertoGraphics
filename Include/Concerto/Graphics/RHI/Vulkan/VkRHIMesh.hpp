//
// Created by arthur on 17/02/2023.
//

#ifndef CONCERTO_GRAPHICS_RHI_VKRHIMESH_HPP
#define CONCERTO_GRAPHICS_RHI_VKRHIMESH_HPP

#include <unordered_map>
#include <memory>

#include "Concerto/Graphics/UploadContext.hpp"
#include "Concerto/Graphics/RHI/MaterialBuilder.hpp"
#include "Concerto/Graphics/Defines.hpp"
#include "Concerto/Graphics/RHI/Material.hpp"
#include "Concerto/Graphics/RHI/Mesh.hpp"
#include "Concerto/Graphics/RHI/SubMesh.hpp"

namespace Concerto::Graphics::Vk
{
	class GpuMesh;
}

namespace Concerto::Graphics::RHI
{
	class CONCERTO_GRAPHICS_API VkRHIMesh : public RHI::Mesh
	{
	public:
		explicit VkRHIMesh(std::string filePath);
		explicit VkRHIMesh(Vertices vertices);

		std::shared_ptr<RHI::GpuMesh> BuildGpuMesh(RHI::MaterialBuilder& materialBuilder, const RHI::RenderPass& renderPass, RHI::Device& device) override;
	};
	using MeshPtr = std::shared_ptr<VkRHIMesh>;
}

#endif //CONCERTO_GRAPHICS_RHI_VKRHIMESH_HPP
