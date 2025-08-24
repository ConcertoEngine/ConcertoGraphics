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
#include "Concerto/Graphics/RHI/Vulkan/VkRHITextureBuilder.hpp"

#include <Nazara/Core/TaskScheduler.hpp>

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

	std::unique_ptr<rhi::GpuMesh> VkRHIMesh::BuildGpuMesh(rhi::MaterialBuilder& materialBuilder, const rhi::RenderPass& renderPass, rhi::Device& device)
	{
		CCT_GFX_AUTO_PROFILER_SCOPE();

		auto gpuMesh = std::make_unique<rhi::GpuMesh>();

		auto& meshes = this->GetSubMeshes();
		rhi::VkRHIDevice& rhiDevice = Cast<rhi::VkRHIDevice&>(device);
		vk::UploadContext& uploadContext = rhiDevice.GetUploadContext();

		uploadContext.ReserveSecondaryCommandBuffers(meshes.size());

		Nz::TaskScheduler taskScheduler(std::thread::hardware_concurrency());

		std::atomic<std::size_t> totalVertices = 0;
		{
			std::mutex subMeshesMutex;
			CCT_GFX_PROFILER_SCOPE("Load all submeshes");
			for (auto& subMesh : meshes)
			{
				taskScheduler.AddTask([&](){
					totalVertices += subMesh->GetVertices().size();
					auto& materialInfo = *subMesh->GetMaterial();
					materialInfo.vertexShaderPath = "./Shaders/tri_mesh_ssbo.nzsl";
					materialInfo.fragmentShaderPath = materialInfo.diffuseTexturePath.empty() ? "./Shaders/default_lit.nzsl" : "./Shaders/textured_lit.nzsl";

					rhi::MaterialPtr litMaterial = materialBuilder.BuildMaterial(materialInfo, renderPass);

					auto vkSubMesh = std::make_shared<VkRHIGpuSubMesh>(subMesh, litMaterial, rhiDevice);
					std::lock_guard _(subMeshesMutex);
					gpuMesh->subMeshes.push_back(vkSubMesh);
				});
			}
		}

		taskScheduler.WaitForTasks();

		{
			CCT_GFX_PROFILER_SCOPE("Sort sub meshes by material");
			phmap::flat_hash_map<std::size_t, std::vector<GpuSubMeshPtr>> subMeshesByMaterial;
			for (auto& subMesh : gpuMesh->subMeshes)
			{
				auto hash = MaterialInfo::Hash()(*subMesh->GetMaterial());
				auto it = subMeshesByMaterial.find(hash);
				if (it == subMeshesByMaterial.end())
				{
					subMeshesByMaterial.emplace(hash, std::vector{subMesh});
				}
				else
				{
					it->second.emplace_back(subMesh);
				}
			}

			gpuMesh->subMeshes.clear();

			for (auto& [hash, subMeshes] : subMeshesByMaterial)
			{
				for (auto& subMesh : subMeshes)
				{
					gpuMesh->subMeshes.emplace_back(subMesh);
				}
			}
		}

		
		vk::Buffer stagingBuffer(rhiDevice.GetAllocator().AllocateBuffer<Vertex>(totalVertices * sizeof(Vertex), VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VMA_MEMORY_USAGE_CPU_ONLY, true));

		VkRHITextureBuilder::Instance()->Commit();

		uploadContext._commandBuffer.Begin();
		{
			CCT_GFX_PROFILER_SCOPE("Copy vertices");
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