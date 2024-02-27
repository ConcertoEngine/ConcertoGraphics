//
// Created by arthur on 12/07/2022.
//
#include <vulkan/vulkan.h>
#define GLFW_INCLUDE_VULKAN
#include <Concerto/Graphics/Window/Window.hpp>
#include <Concerto/Graphics/Frame.hpp>
#include <Concerto/Graphics/Vulkan/Wrapper/Allocator.hpp>
#include <Concerto/Graphics/Vulkan/Wrapper/Device.hpp>
#include <Concerto/Graphics/Vulkan/Wrapper/RenderPass.hpp>
#include <Concerto/Graphics/Vulkan/Wrapper/ShaderModule.hpp>
#include <Concerto/Graphics/Vulkan/Wrapper/PipelineLayout.hpp>
#include <Concerto/Graphics/Vulkan/Wrapper/Sampler.hpp>
#include <Concerto/Graphics/Vulkan/Wrapper/Swapchain.hpp>
#include <Concerto/Graphics/Vulkan/Wrapper/Pipeline.hpp>
#include <Concerto/Graphics/Vulkan/Wrapper/VulkanInitializer.hpp>
#include <Concerto/Graphics/Vulkan/Wrapper/Queue.hpp>
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
#include <Concerto/Core/Logger.hpp>
#include <Concerto/Core/Math/Quaternion.hpp>
#include <Concerto/Core/Math/Vector.hpp>
#include <Concerto/Core/Math/EulerAngles.hpp>
#include <Concerto/Graphics/Camera.hpp>
#include <Concerto/Graphics/Window/Input.hpp>
#include "Concerto/Graphics/TextureBuilder.hpp"
#include <Concerto/Graphics/ImGUI.hpp>

#include <Vulkan.hpp>
#include <GLFW/glfw3.h>


using namespace Concerto;
using namespace Concerto::Graphics;

void PrintPhysicalDeviceProperties(const PhysicalDevice& physicalDevice)
{
	VkPhysicalDeviceProperties deviceProperties = physicalDevice.GetProperties();

	Logger::Info("Physical Device Properties:");
	Logger::Info("--------------------------------");
	Logger::Info("Device Name: {}", deviceProperties.deviceName);
	Logger::Info("API Version: {}.{}.{}", VK_VERSION_MAJOR(deviceProperties.apiVersion), VK_VERSION_MINOR(deviceProperties.apiVersion), VK_VERSION_PATCH(deviceProperties.apiVersion));
	Logger::Info("Driver Version: {}", deviceProperties.driverVersion);
	Logger::Info("Vendor ID: ", deviceProperties.vendorID);
	Logger::Info("Device ID: ", deviceProperties.deviceID);
	std::string type("Device Type: ");
	switch (deviceProperties.deviceType) {
	case VK_PHYSICAL_DEVICE_TYPE_OTHER:
		type += "Other";
		break;
	case VK_PHYSICAL_DEVICE_TYPE_INTEGRATED_GPU:
		type += "Integrated GPU";
		break;
	case VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU:
		type += "Discrete GPU";
		break;
	case VK_PHYSICAL_DEVICE_TYPE_VIRTUAL_GPU:
		type += "Virtual GPU";
		break;
	case VK_PHYSICAL_DEVICE_TYPE_CPU:
		type += "CPU";
		break;
	default:
		type += "Unknown";
		break;
	}
	Logger::Info("{}", type);

	Logger::Info("--------------------------------");
}

const char* GetMemoryTypeString(UInt32 memoryType) {
	switch (memoryType) {
	case VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT:
		return "Device Local";
	case VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT:
		return "Host Visible";
	case VK_MEMORY_PROPERTY_HOST_COHERENT_BIT:
		return "Host Coherent";
	case VK_MEMORY_PROPERTY_HOST_CACHED_BIT:
		return "Host Cached";
	case VK_MEMORY_PROPERTY_LAZILY_ALLOCATED_BIT:
		return "Lazy Allocated";
	case VK_MEMORY_PROPERTY_PROTECTED_BIT:
		return "Protected";
	case VK_MEMORY_PROPERTY_DEVICE_COHERENT_BIT_AMD:
		return "Device Coherent";
	case VK_MEMORY_PROPERTY_DEVICE_UNCACHED_BIT_AMD:
		return "Device Uncached";
	case VK_MEMORY_PROPERTY_RDMA_CAPABLE_BIT_NV:
		return "RDMA Capable";
	default:
		return "Unknown";
	}
}

const char* GetMemoryHeapFlagsString(UInt32 heapFlags) {
	if (heapFlags & VK_MEMORY_HEAP_DEVICE_LOCAL_BIT)
		return "Device Local";
	if (heapFlags & VK_MEMORY_HEAP_MULTI_INSTANCE_BIT)
		return "Multi-Instance";
	return "Unknown";
}

void PrintPhysicalDeviceMemoryProperties(const PhysicalDevice& physicalDevice) {
	VkPhysicalDeviceMemoryProperties memoryProperties = physicalDevice.GetMemoryProperties();

	Logger::Info("Physical Device Memory Properties:");
	Logger::Info("--------------------------------");
	Logger::Info("Memory Types: {}", memoryProperties.memoryTypeCount);
	for (UInt32 i = 0; i < memoryProperties.memoryTypeCount; ++i)
		Logger::Info("  Type {}: Heap Index {}", GetMemoryTypeString(i), memoryProperties.memoryTypes[i].heapIndex);

	Logger::Info("Memory Heaps: {}", memoryProperties.memoryHeapCount);
	for (uint32_t i = 0; i < memoryProperties.memoryHeapCount; ++i)
	{
		Logger::Info("  Heap {}: Size {} MB ({} GB), Flags {}", std::to_string(i),
			std::to_string(static_cast<float>(memoryProperties.memoryHeaps[i].size) / (1024.f * 1024.f)),
			std::to_string(static_cast<float>(memoryProperties.memoryHeaps[i].size) / (1024.f * 1024.f * 1024.f)),
			GetMemoryHeapFlagsString(memoryProperties.memoryHeaps[i].flags));

	}

	Logger::Info("--------------------------------");
}


int main()
{
	try
	{
		RendererInfo info = {
			.applicationName = "Test",
			.applicationVersion = { 1, 0, 0 },
			.width = 1280,
			.height = 720,
		};
		Window window("Concerto Graphics", 1280, 720);
		Input& inputManager = window.GetInputManager();
		Vulkan vulkan(info);
		auto& vkInstance = vulkan.GetVkInstance();
		Device* device = vulkan.CreateDevice(DeviceType::Dedicated);
		PrintPhysicalDeviceProperties(device->GetPhysicalDevice());
		PrintPhysicalDeviceMemoryProperties(device->GetPhysicalDevice());
		auto memoryProperties = device->GetPhysicalDevice().GetMemoryProperties();

		Allocator& allocator = device->GetAllocator();
		Queue& graphicsQueue = device->GetQueue(Queue::Type::Graphics);
		UploadContext uploadContext(*device, device->GetQueueFamilyIndex(Queue::Type::Graphics));

		auto minimumAlignment = device->GetPhysicalDevice().GetProperties().limits.minUniformBufferOffsetAlignment;
		allocator.SetDevice(device);
		VkSurfaceKHR surface = {};
		Input input;
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
		TextureBuilder textureBuilder(*device, uploadContext._commandBuffer, uploadContext, graphicsQueue);

		Mesh mesh("./assets/sponza/sponza.obj");

		std::vector<FrameData> frames;
		frames.reserve(swapchain.GetImages().size());
		for (std::size_t i = 0; i < frames.capacity(); i++)
			frames.emplace_back(*device);

		auto vkMesh = std::make_shared<VkMesh>();
		for (auto& subMesh : mesh.GetSubMeshes())
		{
			MaterialInfo materialInfo = *subMesh->GetMaterial();
			materialInfo.vertexShaderPath = "./Shaders/tri_mesh_ssbo.nzsl";
			materialInfo.fragmentShaderPath = materialInfo.diffuseTexturePath.empty() ? "./Shaders/default_lit.nzsl" : "./Shaders/textured_lit.nzsl";
			VkMaterialPtr litMaterial = materialBuilder.BuildMaterial(materialInfo, *renderPass);
			auto vkSubMesh = std::make_shared<VkSubMesh>(subMesh, allocator, VK_BUFFER_USAGE_VERTEX_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT, VMA_MEMORY_USAGE_GPU_ONLY, litMaterial);
			vkSubMesh->Upload(uploadContext._commandBuffer, uploadContext._commandPool, uploadContext._uploadFence, graphicsQueue, allocator);

			vkMesh->subMeshes.push_back(vkSubMesh);
		}

		const Vector3f position(0.f, 0.f, 0.f);
		const EulerAnglesf rotation(0, 0, 0);
		const Vector3f scale(10.f, 10.f, 10.f);
		auto modelMatrix = Matrix4f::Identity();
		modelMatrix *= position.ToTranslationMatrix();
		modelMatrix *= rotation.ToQuaternion().ToRotationMatrix<Matrix4f>();
		modelMatrix *= scale.ToScalingMatrix();

		float aspect = static_cast<float>(window.GetWidth()) / static_cast<float>(window.GetHeight());
		float deltaTime = 0.f;
		Camera camera(ToRadians(90.f), 0.1f, 1000000.f, aspect);
		bool cursorDisabled = false;
		float speed = 15000.f;
		window.SetCursorDisabled(cursorDisabled);
		window.RegisterResizeCallback([&](Window& window)
		{
			aspect = static_cast<float>(window.GetWidth()) / static_cast<float>(window.GetHeight());
			camera.SetAspectRatio(aspect);
			camera.SetFov(45.f);
			camera.SetNear(0.0001f);
			camera.SetFar(1000.f);
		});
		inputManager.Register("MouseMoved", MouseEvent::Type::Moved, [&camera](const MouseEvent& e)
		{
			camera.Rotate(e.mouseMove.deltaX, -e.mouseMove.deltaY);
		});

		inputManager.Register("Forward", Key::Z, TriggerType::Held, [&camera, &speed, &deltaTime]()
		{
			camera.Move(Camera::CameraMovement::Forward, deltaTime * speed);
		});

		inputManager.Register("Backward", Key::S, TriggerType::Held, [&camera, &speed, &deltaTime]()
		{
			camera.Move(Camera::CameraMovement::Backward, deltaTime * speed);
		});

		inputManager.Register("Left", Key::Q, TriggerType::Held, [&camera, &speed, &deltaTime]()
		{
			camera.Move(Camera::CameraMovement::Left, deltaTime * speed);
		});

		inputManager.Register("Right", Key::D, TriggerType::Held, [&camera, &speed, &deltaTime]()
		{
			camera.Move(Camera::CameraMovement::Right, deltaTime * speed);
		});

		inputManager.Register("MouseFocused", Key::LeftAlt, TriggerType::Pressed, [&cursorDisabled, &window]()
		{
			cursorDisabled = !cursorDisabled;
			window.SetCursorDisabled(cursorDisabled);
		});
		std::size_t frameNumber = 0;

		Scene sceneParameters = {};
		sceneParameters.gpuSceneData.sunlightDirection = Vector4f{ 3.1f, 1.f, -1.f, 0 };
		sceneParameters.gpuSceneData.ambientColor = Vector4f{ 0.f, 0.f, 0.f, 1.f };
		sceneParameters.gpuSceneData.sunlightColor = Vector4f{ 255.f, 109.f, 39.f, 1.f };
		sceneParameters.clearColor = Vector4f{ 0.1f, 0.1f, 0.1f, 1.f };

		Graphics::Buffer cameraBuffer(MakeBuffer<GPUCamera>(allocator, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, VMA_MEMORY_USAGE_CPU_TO_GPU));
		Graphics::Buffer sceneBuffer(MakeBuffer<Scene>(allocator, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, VMA_MEMORY_USAGE_CPU_TO_GPU));
		Graphics::Buffer objectsBuffer(MakeBuffer<GPUObjectData>(allocator, VK_BUFFER_USAGE_STORAGE_BUFFER_BIT, VMA_MEMORY_USAGE_CPU_TO_GPU));
		auto materials = materialBuilder.GetMaterials();
		for (auto& material : materials)
		{
			{
				VkDescriptorBufferInfo bufferInfo;
				bufferInfo.buffer = *cameraBuffer.Get();
				bufferInfo.offset = 0;
				bufferInfo.range = cameraBuffer.GetAllocatedSize();

				VkWriteDescriptorSet cameraWrite = VulkanInitializer::WriteDescriptorBuffer(
					VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,
					*material->descriptorSets[0]->Get(), &bufferInfo, 0);
				device->UpdateDescriptorSetWrite(cameraWrite);
			}
			{
				VkDescriptorBufferInfo bufferInfo;
				bufferInfo.buffer = *sceneBuffer.Get();
				bufferInfo.offset = 0;
				bufferInfo.range = sceneBuffer.GetAllocatedSize();

				VkWriteDescriptorSet sceneWrite = VulkanInitializer::WriteDescriptorBuffer(
					VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,
					*material->descriptorSets[0]->Get(), &bufferInfo, 1);

				device->UpdateDescriptorSetWrite(sceneWrite);
			}
			{
				VkDescriptorBufferInfo bufferInfo;
				bufferInfo.buffer = *objectsBuffer.Get();
				bufferInfo.offset = 0;
				bufferInfo.range = objectsBuffer.GetAllocatedSize();

				VkWriteDescriptorSet objectWrite = VulkanInitializer::WriteDescriptorBuffer(
					VK_DESCRIPTOR_TYPE_STORAGE_BUFFER,
					*material->descriptorSets[1]->Get(), &bufferInfo, 0);

				device->UpdateDescriptorSetWrite(objectWrite);
			}
		}


		auto transfersFunction = [&](FrameData& frame, UInt32 uniformOffset) {
			cameraBuffer.Copy<GPUCamera>(camera);
			sceneBuffer.Copy(sceneParameters.gpuSceneData, PadUniformBuffer(sizeof(GPUSceneData), minimumAlignment * (frameNumber % 2)));
			objectsBuffer.Copy(modelMatrix);
			auto* drawIndirectCommand = frame.indirectBuffer.Map<VkDrawIndirectCommand>();
			UInt32 i = 0;
			for (const auto& subMesh : mesh.GetSubMeshes())
			{
				VkDrawIndirectCommand* drawIndirectCommandPtr = &drawIndirectCommand[i];
				drawIndirectCommandPtr->vertexCount = subMesh->GetVertices().size();
				drawIndirectCommandPtr->instanceCount = 1;
				drawIndirectCommandPtr->firstVertex = 0;
				drawIndirectCommandPtr->firstInstance = 0;
				++i;
			}
			frame.indirectBuffer.UnMap();
			i = 0;
			PipelinePtr lasPipeline;
			for (const auto& subMesh : vkMesh->subMeshes)
			{
				const auto vkMaterial = subMesh->material;
				if (vkMaterial == nullptr)
				{
					Logger::Error("Could not found the material {}", subMesh->GetMaterial()->name);
					CONCERTO_ASSERT_FALSE;
					continue;
				}
				if (!lasPipeline || lasPipeline != vkMaterial->pipeline)
				{
					frame.commandBuffer.BindPipeline(VK_PIPELINE_BIND_POINT_GRAPHICS, *vkMaterial->pipeline->Get());
					lasPipeline = vkMaterial->pipeline;
				}
				frame.commandBuffer.BindDescriptorSets(VK_PIPELINE_BIND_POINT_GRAPHICS, *vkMaterial->pipeline->GetPipelineLayout()->Get(), vkMaterial->descriptorSets);
				frame.commandBuffer.BindVertexBuffers(subMesh->vertexBuffer);
				frame.commandBuffer.DrawIndirect(frame.indirectBuffer, sizeof(VkDrawIndirectCommand) * i, drawIndirectCommand[i].instanceCount, sizeof(VkDrawIndirectCommand));
				++i;
			}
			};

		VkViewport viewport{
			.width = static_cast<float>(info.width),
			.height = static_cast<float>(info.height),
			.minDepth = 0.0f,
			.maxDepth = 1.0f,
		};
		VkRect2D dynamicScissor = {};
		dynamicScissor.offset = { 0, 0 };
		dynamicScissor.extent = { info.width, info.height };

		auto presentFunction = [&](FrameData& frame, const UInt32 uniformOffset)
			{
				frame.renderFence.Wait(-1);
				frame.renderFence.Reset();
				VkClearValue clearValue;
				clearValue.color = {
			{
						sceneParameters.clearColor.X(),
						sceneParameters.clearColor.Y(),
						sceneParameters.clearColor.Z(),
						1.0f
					}
				};
				VkClearValue depthClear;
				depthClear.depthStencil.depth = 1.f;
				const VkClearValue clearValues[] = { clearValue, depthClear };
				VkRenderPassBeginInfo rpInfo = VulkanInitializer::RenderPassBeginInfo(*renderPass->Get(), { info.width, info.height }, *swapchain.GetCurrentFrameBuffer().Get());
				rpInfo.clearValueCount = 2;
				rpInfo.pClearValues = &clearValues[0];
				frame.commandBuffer.Reset();
				frame.commandBuffer.Begin();
				{
					frame.commandBuffer.SetViewport(viewport);
					frame.commandBuffer.SetScissor(dynamicScissor);
					frame.commandBuffer.BeginRenderPass(rpInfo);
					{
						transfersFunction(frame, uniformOffset);
					}
					frame.commandBuffer.EndRenderPass();
				}
				frame.commandBuffer.End();

				graphicsQueue.Submit(frame);
				if (!graphicsQueue.Present(frame, swapchain, swapchain.GetCurrentImageIndex()))
				{
					CONCERTO_ASSERT_FALSE; // need resize
					return;
				}
			};
		std::chrono::steady_clock::time_point lastFrameTime = std::chrono::steady_clock::now();
		while (!window.ShouldClose())
		{
			camera.UpdateViewProjectionMatrix();
			auto beginTime = std::chrono::high_resolution_clock::now();
			deltaTime = std::chrono::duration<float>(beginTime - lastFrameTime).count();
			lastFrameTime = beginTime;
			int fps = 1.f / deltaTime;
			window.SetTitle(info.applicationName + " - " + std::to_string(fps) + " fps - " + std::to_string(frameNumber) + " frames");
			window.PopEvent();
			UInt32 uniformOffset = PadUniformBuffer(sizeof(GPUSceneData), minimumAlignment) * frameNumber;
			FrameData& currentFrame = frames[frameNumber % frames.size()];
			swapchain.AcquireNextImage(currentFrame.presentSemaphore, currentFrame.renderFence, 999999999);
			presentFunction(currentFrame, uniformOffset);
			frameNumber++;
		}
	}
	catch (const std::exception& e)
	{
		std::cout << e.what() << std::endl;
	}
	return 0;
}
