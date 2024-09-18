//
// Created by arthur on 08/08/2024.
//

#include <filesystem>
#include <Concerto/Core/Cast.hpp>

#include "Concerto/Graphics/RHI/Vulkan/VkRHIMesh.hpp"
#include "Concerto/Graphics/Rhi/Vulkan/VkRHIGpuSubMesh.hpp"
#include "Concerto/Graphics/RHI/Vulkan/VkRHIDevice.hpp"
#include "Concerto/Graphics/RHI/GpuMesh.hpp"

namespace Concerto::Graphics::RHI
{
	VkRHIMesh::VkRHIMesh(std::string filePath) :
		Mesh(std::move(filePath))
	{
	}

	VkRHIMesh::VkRHIMesh(Vertices vertices) :
		Mesh(std::move(vertices))
	{
	}

	std::shared_ptr<RHI::GpuMesh> VkRHIMesh::BuildGpuMesh(RHI::MaterialBuilder& materialBuilder, const RHI::RenderPass& renderPass, RHI::Device& device)
	{
		auto gpuMesh = std::make_shared<RHI::GpuMesh>();

		auto& meshes = this->GetSubMeshes();
		for (auto& subMesh : meshes)
		{
			auto& materialInfo = *subMesh->GetMaterial();
			materialInfo.vertexShaderPath = "./Shaders/tri_mesh_ssbo.nzsl";
			materialInfo.fragmentShaderPath = materialInfo.diffuseTexturePath.empty() ? "./Shaders/default_lit.nzsl" : "./Shaders/textured_lit.nzsl";

			RHI::MaterialPtr litMaterial = materialBuilder.BuildMaterial(materialInfo, renderPass);
			RHI::VkRHIDevice& rhiDevice = Cast<RHI::VkRHIDevice&>(device);
			Vk::UploadContext& uploadContext = rhiDevice.GetUploadContext();

			auto vkSubMesh = std::make_shared<VkRHIGpuSubMesh>(subMesh, litMaterial, rhiDevice);
			auto start = std::chrono::system_clock::now();
			vkSubMesh->Upload(uploadContext._commandBuffer, uploadContext._commandPool, uploadContext._uploadFence, rhiDevice.GetQueue(Vk::Queue::Type::Graphics), Cast<RHI::VkRHIDevice&>(device));

			gpuMesh->subMeshes.push_back(vkSubMesh);
		}
		return gpuMesh;
	}
} // namespace Concerto::Graphics