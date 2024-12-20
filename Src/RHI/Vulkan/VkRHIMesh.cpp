//
// Created by arthur on 08/08/2024.
//

#include <filesystem>
#include <Concerto/Core/Cast.hpp>

#include "Concerto/Graphics/RHI/Vulkan/VkRHIMesh.hpp"
#include "Concerto/Graphics/Rhi/Vulkan/VkRHIGpuSubMesh.hpp"
#include "Concerto/Graphics/RHI/Vulkan/VkRHIDevice.hpp"
#include "Concerto/Graphics/RHI/GpuMesh.hpp"
#include "Concerto/Graphics/RHI/Vulkan/VkRHIBuffer.hpp"

namespace cct::gfx::rhi
{
	VkRHIMesh::VkRHIMesh(std::string filePath) :
		Mesh(std::move(filePath))
	{
	}

	VkRHIMesh::VkRHIMesh(Vertices vertices) :
		Mesh(std::move(vertices))
	{
	}

	std::shared_ptr<rhi::GpuMesh> VkRHIMesh::BuildGpuMesh(rhi::MaterialBuilder& materialBuilder, const rhi::RenderPass& renderPass, rhi::Device& device)
	{
		auto gpuMesh = std::make_shared<rhi::GpuMesh>();

		auto& meshes = this->GetSubMeshes();
		rhi::VkRHIDevice& rhiDevice = Cast<rhi::VkRHIDevice&>(device);
		vk::UploadContext& uploadContext = rhiDevice.GetUploadContext();

		std::size_t totalVertices = 0;
		for (auto& subMesh : meshes)
		{
			totalVertices += subMesh->GetVertices().size();
			auto& materialInfo = *subMesh->GetMaterial();
			materialInfo.vertexShaderPath = "./Shaders/tri_mesh_ssbo.nzsl";
			materialInfo.fragmentShaderPath = materialInfo.diffuseTexturePath.empty() ? "./Shaders/default_lit.nzsl" : "./Shaders/textured_lit.nzsl";

			rhi::MaterialPtr litMaterial = materialBuilder.BuildMaterial(materialInfo, renderPass);

			auto vkSubMesh = std::make_shared<VkRHIGpuSubMesh>(subMesh, litMaterial, rhiDevice);
			gpuMesh->subMeshes.push_back(vkSubMesh);
		}

		vk::Buffer stagingBuffer(vk::MakeBuffer<Vertex>(rhiDevice.GetAllocator(), totalVertices * sizeof(Vertex), VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VMA_MEMORY_USAGE_CPU_ONLY, true));

		uploadContext._commandBuffer.Begin();
		{
			std::size_t padding = 0;

			for (auto& gpuMeshSubMesh : gpuMesh->subMeshes)
			{
				const auto& vertexBuffer = Cast<const VkRHIBuffer&>(gpuMeshSubMesh->GetVertexBuffer());
				stagingBuffer.Copy(gpuMeshSubMesh->GetVertices().data(), gpuMeshSubMesh->GetVertices().size() * sizeof(Vertex), padding);
				uploadContext._commandBuffer.CopyBuffer(stagingBuffer, vertexBuffer, gpuMeshSubMesh->GetVertices().size() * sizeof(Vertex), padding, 0);
				padding += gpuMeshSubMesh->GetVertices().size() * sizeof(Vertex);
			}
		}
		uploadContext._commandBuffer.End();

		rhiDevice.GetQueue(vk::Queue::Type::Graphics).Submit(uploadContext._commandBuffer, nullptr, nullptr, uploadContext._uploadFence);
		uploadContext._uploadFence.Wait(9999999999);
		uploadContext._uploadFence.Reset();
		uploadContext._commandPool.Reset();

		return gpuMesh;
	}
} // namespace cct::gfx