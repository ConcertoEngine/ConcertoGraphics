//
// Created by arthur on 12/07/2022.
//
#include <vulkan/vulkan.h>
#define GLFW_INCLUDE_VULKAN
#include <Concerto/Graphics/Vulkan/Wrapper/Allocator.hpp>
#include <Concerto/Graphics/Window/Window.hpp>
#include <Concerto/Graphics/Frame.hpp>
#include <Concerto/Graphics/Vulkan/Wrapper/Device.hpp>
#include <Concerto/Graphics/Vulkan/Wrapper/RenderPass.hpp>
#include <Concerto/Graphics/Vulkan/Wrapper/ShaderModule.hpp>
#include <Concerto/Graphics/Vulkan/Wrapper/PipelineLayout.hpp>
#include <Concerto/Graphics/Vulkan/Wrapper/Sampler.hpp>
#include <Concerto/Graphics/Vulkan/Wrapper/Swapchain.hpp>
#include <Concerto/Graphics/Vulkan/Wrapper/Pipeline.hpp>
#include <Concerto/Graphics/Vulkan/DescriptorBuilder.hpp>
#include <Concerto/Graphics/Vulkan/DescriptorAllocator.hpp>
#include <Concerto/Graphics/Vulkan/DescriptorLayoutCache.hpp>
#include <Concerto/Graphics/Mesh.hpp>
#include <Concerto/Graphics/Primitives.hpp>
#include <Concerto/Graphics/UploadContext.hpp>
#include <Concerto/Graphics/MaterialBuilder.hpp>
#include <Concerto/Graphics/Vulkan/Utils.hpp>
#include <Concerto/Graphics/Vulkan/VkMesh.hpp>
#include <Concerto/Graphics/ShaderReflection.hpp>
#include <Concerto/Graphics/UploadContext.hpp>
#include <Concerto/Graphics/Vulkan/Wrapper/VulkanInitializer.hpp>

#include <Concerto/Core/Logger.hpp>
#include <Concerto/Core/Math/Quaternion.hpp>
#include <Concerto/Core/Math/Vector.hpp>
#include <Concerto/Core/Math/EulerAngles.hpp>

#include <Vulkan.hpp>



using namespace Concerto;
using namespace Concerto::Graphics;

int main()
{
	try
	{
		RendererInfo info ={
			.applicationName = "Test",
			.applicationVersion = { 1, 0, 0 },
			.width = 1280,
			.height = 720,
		};
		Window window("Concerto Graphics", 1280, 720);
		Vulkan vulkan(info);
		auto& vkInstance = vulkan.GetVkInstance();
		Device* device = vulkan.CreateDevice(DeviceType::Dedicated);
		Allocator& allocator = device->GetAllocator();
		auto minimumAlignment = device->GetPhysicalDevice().GetProperties().limits.minUniformBufferOffsetAlignment;
		allocator.SetDevice(device);
		VkSurfaceKHR surface = {};
		if (!window.IsVulkanSupported())
		{
			Logger::Error("Vulkan is not supported");
			return 1;
		}
		if (!window.CreateVulkanSurface(vkInstance, &surface))
		{
			Logger::Error("Cannot create vulkan surface");
			return 1;
		}
		
		device->GetPhysicalDevice().SetSurface(surface);
		Swapchain swapchain(*device, window);
		RenderPass* renderPass = swapchain.GetRenderPass();
		MaterialBuilder materialBuilder(*device);

		MaterialInfo materialInfo;
		materialInfo.vertexShaderPath = "./Shaders/tri_mesh_ssbo.nzsl";
		materialInfo.fragmentShaderPath = "./Shaders/default_lit.nzsl";

		VkMaterialPtr litMaterial = materialBuilder.BuildMaterial(materialInfo, *renderPass);
		std::vector<FrameData> frames;
		frames.reserve(swapchain.GetImages().size());
		for (std::size_t i = 0; i < frames.capacity(); i++)
			frames.emplace_back(*device);

		Queue& graphicsQueue = device->GetQueue(Graphics::Queue::Type::Graphics);
		auto& uploadContext = device->GetUploadContext();

		Mesh mesh(Primitive::MakeCube(5));
		const Math::Vector3f position(0.f, 0.f, 0.f);
		const Math::EulerAnglesf rotation(0, 0, 0);
		const Math::Vector3f scale(1, 1, 1);
		glm::mat4 modelMatrix = glm::mat4(1.0f);
		modelMatrix = glm::translate(modelMatrix, glm::vec3(position.X(), position.Y(), position.Z()));
		modelMatrix = glm::rotate(modelMatrix, glm::radians(rotation.Yaw()), glm::vec3(1.0f, 0.0f, 0.0f));
		modelMatrix = glm::rotate(modelMatrix, glm::radians(rotation.Pitch()), glm::vec3(0.0f, 1.0f, 0.0f));
		modelMatrix = glm::rotate(modelMatrix, glm::radians(rotation.Roll()), glm::vec3(0.0f, 0.0f, 1.0f));
		modelMatrix = glm::scale(modelMatrix, glm::vec3(scale.X(), scale.Y(), scale.Z()));
		
		std::size_t frameNumber = 0;
		

		struct UBO
		{
			glm::mat4 model;
		};

		GPUCamera camera = {};

		Scene sceneParameters = {};
		sceneParameters.gpuSceneData.sunlightDirection = { 3.1f, 1.f, -1.f, 0 };
		sceneParameters.gpuSceneData.ambientColor = { 0.f, 0.f, 0.f, 1.f };
		sceneParameters.gpuSceneData.sunlightColor = { 255.f, 109.f, 39.f, 1.f };
		sceneParameters.clearColor = { 0.1f, 0.1f, 0.1f, 1.f };
		
		auto transfersFunction = [&](FrameData& frame, UInt32 uniformOffset){
			{
				Graphics::Buffer buffer(MakeBuffer<GPUCamera>(allocator, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, VMA_MEMORY_USAGE_CPU_TO_GPU));
				buffer.Copy<GPUCamera>(camera);
				VkDescriptorBufferInfo cameraInfo;
				cameraInfo.buffer = buffer._buffer;
				cameraInfo.offset = 0;
				cameraInfo.range = sizeof(GPUCamera);

				VkWriteDescriptorSet cameraWrite = VulkanInitializer::WriteDescriptorBuffer(
					VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,
					*litMaterial->_descriptorSets[0]->descriptorSet.Get(), &cameraInfo, 0);

				device->UpdateDescriptorSetWrite(cameraWrite);
			}
			{
				Graphics::Buffer buffer(MakeBuffer<Scene>(allocator, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, VMA_MEMORY_USAGE_CPU_TO_GPU));
				buffer.Copy<GPUSceneData>(sceneParameters.gpuSceneData);
				VkDescriptorBufferInfo cameraInfo;
				cameraInfo.buffer = buffer._buffer;
				cameraInfo.offset = 0;
				cameraInfo.range = sizeof(Scene);

				VkWriteDescriptorSet cameraWrite = VulkanInitializer::WriteDescriptorBuffer(
					VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,
					*litMaterial->_descriptorSets[0]->descriptorSet.Get(), &cameraInfo, 0);

				device->UpdateDescriptorSetWrite(cameraWrite);
			}
			
			auto* drawIndirectCommand = frame._indirectBuffer.Map<VkDrawIndirectCommand>();
			std::size_t i = 0;
			for (auto& subMesh : mesh.GetSubMeshes())
			{
				VkDrawIndirectCommand* drawIndirectCommandPtr = &drawIndirectCommand[i];
				drawIndirectCommandPtr->vertexCount = subMesh->GetVertices().size();
				drawIndirectCommandPtr->instanceCount = 1;
				drawIndirectCommandPtr->firstVertex = 0;
				drawIndirectCommandPtr->firstInstance = 0;				
				++i;
			}
			frame._indirectBuffer.UnMap();
			
			i = 0;
			for (auto& subMesh : mesh.GetSubMeshes())
			{
				auto vkMaterial = materialBuilder.GetMaterial(subMesh->GetMaterial()->name);
				frame._mainCommandBuffer->BindPipeline(VK_PIPELINE_BIND_POINT_GRAPHICS, *vkMaterial->_pipeline);
				frame._mainCommandBuffer->BindDescriptorSets(VK_PIPELINE_BIND_POINT_GRAPHICS, *vkMaterial->_pipelineLayout->Get(), 0, 1, vkMaterial->_descriptorSets[0]->descriptorSet, uniformOffset);
				//frame._mainCommandBuffer->BindDescriptorSets(VK_PIPELINE_BIND_POINT_GRAPHICS, *vkMaterial->_pipelineLayout->Get(), 1, 1, frame.objectDescriptor);
				frame._mainCommandBuffer->DrawIndirect(frame._indirectBuffer, sizeof(VkDrawIndirectCommand) * i, drawIndirectCommand[i].instanceCount, sizeof(VkDrawIndirectCommand));
				++i;
			}
		};
		
		while (!window.ShouldClose())
		{
			window.PopEvent();
			UInt32 uniformOffset = PadUniformBuffer(sizeof(GPUSceneData), minimumAlignment) * frameNumber;
			FrameData& currentFrame = frames[frameNumber % frames.size()];
			transfersFunction(currentFrame, uniformOffset);
			frameNumber++;
		}
	}
	catch (const std::exception& e)
	{
		std::cout << e.what() << std::endl;
	}
	return 0;
}
