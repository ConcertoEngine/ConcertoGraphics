//
// Created by arthur on 12/07/2022.
//


#include <chrono>
#include <Concerto/Core/Math/Algorithm.hpp>
#include <Concerto/Graphics/Window/Window.hpp>
#include <Concerto/Graphics/RHI/APIImpl.hpp>
#include <Concerto/Graphics/RHI/Instance.hpp>
#include <Concerto/Graphics/RHI/SwapChain.hpp>
#include <Concerto/Graphics/RHI/RenderPass.hpp>
#include <Concerto/Graphics/RHI/FrameBuffer.hpp>
#include <Concerto/Graphics/RHI/Texture.hpp>
#include <Concerto/Graphics/RHI/MaterialBuilder.hpp>
#include <Concerto/Graphics/RHI/CommandBuffer.hpp>

#include <Concerto/Graphics/RHI/Frame.hpp>
#include <Concerto/Graphics/RHI/Mesh.hpp>
#include <Concerto/Graphics/RHI/Vulkan/VkRHIMesh.hpp>

#include "Concerto/Graphics/Camera.hpp"
#include "Concerto/Graphics/GPUData.hpp"
#include "Concerto/Graphics/RHI/GpuMesh.hpp"
#include "Concerto/Graphics/RHI/GpuSubMesh.hpp"

using namespace Concerto;
using namespace Concerto::Graphics;

int main()
{
	try
	{
		Window window("Concerto Graphics", 1280, 720);
		Input& inputManager = window.GetInputManager();

		RHI::Instance rInstance;
		std::unique_ptr<RHI::Device> device;

		std::size_t deviceIndex = 0;
		for (const auto& [name, vendor, type] : rInstance.EnumerateDevices())
		{
			if (type == RHI::DeviceType::Dedicated)
			{
				device = rInstance.CreateDevice(deviceIndex);
				break;
			}
			++deviceIndex;
		}
		std::size_t minimumAlignment = device->GetMinimumUniformBufferOffsetAlignment();
		std::unique_ptr<RHI::SwapChain> swapChain = device->CreateSwapChain(window);
		RHI::RenderPass& renderPass = swapChain->GetRenderPass();

		std::unique_ptr<RHI::MaterialBuilder> materialBuilder = device->CreateMaterialBuilder(swapChain->GetExtent());
		std::unique_ptr<RHI::TextureBuilder> textureBuilder = device->CreateTextureBuilder();
		RHI::VkRHIMesh mesh("./assets/sponza/sponza.obj"); //fixme
		std::shared_ptr<RHI::GpuMesh> gpuMesh = mesh.BuildGpuMesh(*materialBuilder, renderPass, *device);

		float aspect = static_cast<float>(window.GetWidth()) / static_cast<float>(window.GetHeight());
		Camera camera(ToRadians(90.f), 0.1f, 1000000.f, aspect);
		bool cursorDisabled = false;
		float deltaTime = 0.f;
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
		Scene sceneParameters = {};
		sceneParameters.gpuSceneData.sunlightDirection = Vector4f{ 3.1f, 1.f, -1.f, 0 };
		sceneParameters.gpuSceneData.ambientColor = Vector4f{ 0.f, 0.f, 0.f, 1.f };
		sceneParameters.gpuSceneData.sunlightColor = Vector4f{ 255.f, 109.f, 39.f, 1.f };
		sceneParameters.clearColor = Vector4f{ 0.1f, 0.1f, 0.1f, 1.f };

		const Vector3f position(0.f, 0.f, 0.f);
		const EulerAnglesf rotation(0, 0, 0);
		const Vector3f scale(1.f, 1.f, 1.f);
		auto modelMatrix = Matrix4f::Identity();
		modelMatrix *= position.ToTranslationMatrix();
		modelMatrix *= rotation.ToQuaternion().ToRotationMatrix<Matrix4f>();
		modelMatrix *= scale.ToScalingMatrix();

		std::unique_ptr<RHI::Buffer> cameraBuffer = device->CreateBuffer(static_cast<RHI::BufferUsageFlags>(RHI::BufferUsage::Uniform), sizeof(GPUCamera), true);
		std::unique_ptr<RHI::Buffer> sceneBuffer = device->CreateBuffer(static_cast<RHI::BufferUsageFlags>(RHI::BufferUsage::Uniform), sizeof(Scene), true);
		std::unique_ptr<RHI::Buffer> objectsBuffer = device->CreateBuffer(static_cast<RHI::BufferUsageFlags>(RHI::BufferUsage::Storage), sizeof(GPUObjectData), true);

		materialBuilder->Update(*cameraBuffer, 0, 0);
		materialBuilder->Update(*sceneBuffer, 0, 1);
		materialBuilder->Update(*objectsBuffer, 1, 0);

		
		std::chrono::steady_clock::time_point lastFrameTime = std::chrono::steady_clock::now();
		while (!window.ShouldClose())
		{
			window.PopEvent();
			camera.UpdateViewProjectionMatrix();
			auto beginTime = std::chrono::high_resolution_clock::now();
			deltaTime = std::chrono::duration<float>(beginTime - lastFrameTime).count();
			lastFrameTime = beginTime;
			RHI::Frame& currentFrame = swapChain->AcquireFrame();
			RHI::CommandBuffer& commandBuffer = currentFrame.GetCommandBuffer();
			cameraBuffer->Write<GPUCamera>(camera, RHI::PadUniformBuffer(sizeof(GPUCamera), minimumAlignment * currentFrame.GetCurrentFrameIndex()));
			sceneBuffer->Write(sceneParameters.gpuSceneData);
			objectsBuffer->Write(modelMatrix);
			commandBuffer.Reset();
			commandBuffer.Begin();
			{
				Viewport viewport{
					.x = 0.f,
					.y = 0.f,
					.width = static_cast<float>(window.GetWidth()),
					.height = static_cast<float>(window.GetHeight()),
					.minDepth = 0.f,
					.maxDepth = 1.f,
				};
				Rect2D dynamicScissor = {};
				dynamicScissor.x = 0;
				dynamicScissor.y = 0;
				dynamicScissor.width = window.GetWidth();
				dynamicScissor.height = window.GetHeight();
				commandBuffer.SetViewport(viewport);
				commandBuffer.SetScissor(dynamicScissor);
				commandBuffer.BeginRenderPass(renderPass, currentFrame.GetFrameBuffer(), Vector3f{ 1.f, 0.f, 0.f });
				{
					RHI::MaterialInfo* lastBoundMaterial = nullptr;
					for (const auto& subMesh : gpuMesh->subMeshes)
					{
						const auto& material = subMesh->GetMaterial();
						if (material == nullptr)
							continue;
						if (lastBoundMaterial != material.get())
						{
							lastBoundMaterial = material.get();
							commandBuffer.BindMaterial(*material);
						}
						commandBuffer.BindVertexBuffer(subMesh->GetVertexBuffer());
						commandBuffer.Draw(static_cast<UInt32>(subMesh->GetVertices().size()), 1, 0, 0);
					}
				}
				commandBuffer.EndRenderPass();
			}
			commandBuffer.End();
			currentFrame.Present();
			window.SetTitle(std::format("ConcertoGraphics - {} fps", 1.f / deltaTime));
		}
	}
	catch (const std::exception& e)
	{
		Logger::Error("An unhandled exception was thrown: '{}'", e.what());
	}
	return 0;
}