//
// Created by arthur on 25/10/2022.
//

#define GLFW_INCLUDE_VULKAN

#include <cassert>
#include <vector>
#include <iostream>
#include <ranges>
#include <filesystem>
#include "glm/gtc/matrix_transform.hpp"
#include "imgui_impl_glfw.h"
#include <GLFW/glfw3.h>

#include "Vulkan/Utils.hpp"
#include "VulkanRenderer.hpp"
#include "window/GlfW3.hpp"
#include "Vulkan/Wrapper/Sampler.hpp"

namespace Concerto::Graphics
{
#if defined(_WIN32)
	std::vector<const char*> extensions = { "VK_KHR_surface", "VK_KHR_win32_surface" };
#elif defined(__linux__)
	std::vector<const char*> extensions = { "VK_KHR_surface", "VK_KHR_xcb_surface", "VK_KHR_xlib_surface", "VK_KHR_wayland_surface" };
#endif
	std::vector<const char*> layers = { "VK_LAYER_KHRONOS_validation" /*, "VK_LAYER_LUNARG_api_dump"*/};
	using namespace Wrapper;

	VulkanRenderer::VulkanRenderer(RendererInfo info, GlfW3& window) : _renderInfo(std::move(info)),
																	   _window(window),
																	   _vulkanInstance(_renderInfo.applicationName,
																		   "Concerto",
																		   { 1, 2, 0 },
																		   _renderInfo.applicationVersion,
																		   { 1, 0, 0 }, extensions, layers)
	{
		assert(_instance == nullptr);
		_instance = this;
		auto res = glfwCreateWindowSurface(*_vulkanInstance.Get(), (GLFWwindow*)_window.GetRawWindow(), nullptr,
			&_surface); //TODO : Move this to Window
		if (res != VK_SUCCESS)
			throw std::runtime_error("Failed to create window surface");
		std::span<PhysicalDevice> devices = _vulkanInstance.EnumeratePhysicalDevices();
		if (devices.empty())
		{
			std::cerr << "No GPU found" << std::endl;
		}
		_physicalDevice = std::move(devices[0]);
		_physicalDevice.SetSurface(_surface);
		_device = Device(_physicalDevice, _deviceExtensions);
		_gpuProperties = _physicalDevice.GetProperties();
		_graphicsQueueFamilyIndex = _device.GetQueue(Queue::Type::Graphics).GetFamilyIndex();
		_allocator = std::make_unique<Allocator>(_physicalDevice, _device, _vulkanInstance);
		_swapchain = std::make_unique<Swapchain>(_device,
			*_allocator,
			VkExtent2D{ _renderInfo.width, _renderInfo.height },
			_physicalDevice);
		_renderPass = std::make_unique<RenderPass>(_device, *_swapchain);
		auto swapchainImagesViews = _swapchain->GetImageViews();
		auto& swapchainDepthImageView = _swapchain->GetDepthImageView();
		for (auto& swapchainImagesView : swapchainImagesViews)
		{
			_frameBuffers.emplace_back(std::move(
				FrameBuffer(_device, *_renderPass, swapchainImagesView, swapchainDepthImageView,
					{ _renderInfo.width, _renderInfo.height })));
		}
		//TODO : Create an abstraction for the descriptor set
		// Commands
		VkDescriptorSetLayoutBinding camBufferBind = VulkanInitializer::DescriptorSetLayoutBinding(
			VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, VK_SHADER_STAGE_VERTEX_BIT, 0);
		VkDescriptorSetLayoutBinding sceneBind = VulkanInitializer::DescriptorSetLayoutBinding(
			VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC, VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT,
			1);
		VkDescriptorSetLayoutBinding objectBind = VulkanInitializer::DescriptorSetLayoutBinding(
			VK_DESCRIPTOR_TYPE_STORAGE_BUFFER, VK_SHADER_STAGE_VERTEX_BIT, 0);
		VkDescriptorSetLayoutBinding textureBind = VulkanInitializer::DescriptorSetLayoutBinding(
			VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, VK_SHADER_STAGE_FRAGMENT_BIT, 0);
		_globalSetLayout = std::make_unique<DescriptorSetLayout>(_device,
			std::vector<VkDescriptorSetLayoutBinding>{ camBufferBind, sceneBind });
		_objectSetLayout =
			std::make_unique<DescriptorSetLayout>(_device, std::vector<VkDescriptorSetLayoutBinding>{ objectBind });
		_singleTextureSetLayout =
			std::make_unique<DescriptorSetLayout>(_device, std::vector<VkDescriptorSetLayoutBinding>{ textureBind });
		_descriptorPool = std::make_unique<DescriptorPool>(_device);
		const std::size_t sceneParamBufferSize =
			2 * PadUniformBuffer(sizeof(GPUSceneData), _gpuProperties.limits.minUniformBufferOffsetAlignment);
		_sceneParameterBuffer =
			std::make_unique<Buffer>(*_allocator, sceneParamBufferSize, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT,
				VMA_MEMORY_USAGE_CPU_TO_GPU);
		_frames.reserve(_swapchain->GetImages().size());
		for (std::size_t i = 0; i < _swapchain->GetImages().size(); i++)
		{
			_frames.emplace_back(std::move(
				FrameData(_device, *_allocator, _graphicsQueueFamilyIndex, *_descriptorPool,
					*_globalSetLayout, *_objectSetLayout, *_sceneParameterBuffer, true)));
		}
		// Commands
		_colorMeshShader = std::make_unique<ShaderModule>(_device, R"(shaders/default_lit.frag.spv)");
		_textureMeshShader = std::make_unique<ShaderModule>(_device, R"(shaders/textured_lit.frag.spv)");
		_meshVertShader = std::make_unique<ShaderModule>(_device, R"(shaders/tri_mesh_ssbo.vert.spv)");
		_meshPipelineLayout = std::make_unique<PipelineLayout>(makePipelineLayout<MeshPushConstants>(_device,
			{ *_globalSetLayout, *_objectSetLayout }));
		_texturedSetLayout = std::make_unique<PipelineLayout>(makePipelineLayout<MeshPushConstants>(_device,
			{ *_globalSetLayout, *_objectSetLayout, *_singleTextureSetLayout }));
		PipelineInfo meshPipelineInfo({ VulkanInitializer::PipelineShaderStageCreateInfo(VK_SHADER_STAGE_VERTEX_BIT,
			*_meshVertShader->Get()), VulkanInitializer::PipelineShaderStageCreateInfo(VK_SHADER_STAGE_FRAGMENT_BIT,
			*_colorMeshShader->Get()) }, { _renderInfo.width, _renderInfo.height }, *_meshPipelineLayout);
		_coloredShaderPipeline = std::make_unique<Pipeline>(Pipeline(_device, meshPipelineInfo));
		_coloredShaderPipeline->BuildPipeline(*_renderPass->Get());
		meshPipelineInfo._shaderStages.clear();
		meshPipelineInfo._shaderStages.push_back(
			VulkanInitializer::PipelineShaderStageCreateInfo(VK_SHADER_STAGE_VERTEX_BIT,
				*_meshVertShader->Get()));
		meshPipelineInfo._shaderStages.push_back(
			VulkanInitializer::PipelineShaderStageCreateInfo(VK_SHADER_STAGE_FRAGMENT_BIT,
				*_textureMeshShader->Get()));
		meshPipelineInfo._pipelineLayout = *_texturedSetLayout->Get();
		_texturedPipeline = std::make_unique<Pipeline>(_device, meshPipelineInfo);
		_texturedPipeline->BuildPipeline(*_renderPass->Get());
		_graphicsQueue = std::make_unique<Queue>(_device, _graphicsQueueFamilyIndex);
		_uploadContext = std::make_unique<UploadContext>(_device, _graphicsQueueFamilyIndex);
		if (_renderInfo.useImGUI)
			UseImGUI();
		_sampler = std::make_unique<Sampler>(_device, VK_FILTER_NEAREST);
		_textureBuilder = std::make_unique<TextureBuilder>(_device, *_allocator, _uploadContext->_commandBuffer,
			*_uploadContext, *_graphicsQueue);
		_descriptorAllocator = std::make_unique<DescriptorAllocator>(_device);
		_descriptorLayoutCache = std::make_unique<DescriptorLayoutCache>(_device);
		_materialBuilder = std::make_unique<MaterialBuilder>(*_descriptorLayoutCache, *_descriptorAllocator, *_sampler);
	}

	VulkanRenderer* VulkanRenderer::Instance()
	{
		assert(_instance != nullptr);
		return _instance;
	}

	void VulkanRenderer::Draw(const Camera& camera)
	{
		if (_imGUI)
			_imGUI->Draw();
		FrameData& frame = _frames[_frameNumber % _frames.size()];
		frame._renderFence.Wait(1000000000);
		frame._renderFence.Reset();
		std::uint32_t swapchainImageIndex = _swapchain->AcquireNextImage(frame._presentSemaphore, frame._renderFence,
			1000000000);
		frame._mainCommandBuffer->Reset();
		frame._mainCommandBuffer->Begin();
		{
			VkClearValue clearValue;
			VkClearValue depthClear;
			clearValue.color = {
				{ _sceneParameters.clearColor.x, _sceneParameters.clearColor.y, _sceneParameters.clearColor.z,
				  1.0f }};
			depthClear.depthStencil.depth = 1.f;
			VkClearValue clearValues[] = { clearValue, depthClear };
			VkRenderPassBeginInfo rpInfo = VulkanInitializer::RenderPassBeginInfo(*_renderPass->Get(),
				{ _renderInfo.width, _renderInfo.height }, *_frameBuffers[swapchainImageIndex].Get());
			rpInfo.clearValueCount = 2;
			rpInfo.pClearValues = &clearValues[0];
			frame._mainCommandBuffer->BeginRenderPass(rpInfo);
			{
				DrawObjects(camera);
				if (_imGUI)
					_imGUI->RenderDrawData(*frame._mainCommandBuffer);
			}
			frame._mainCommandBuffer->EndRenderPass();
		}
		frame._mainCommandBuffer->End();

		_graphicsQueue->Submit(frame);
		if (!_graphicsQueue->Present(frame, *_swapchain, swapchainImageIndex))
		{
			Resize(_window.GetWidth(), _window.GetHeight());
			return;
		}
		_frameNumber++;
		_renderObjectsToDraw.clear();
	}

	void VulkanRenderer::DrawObject(MeshPtr& mesh, const std::string& texturePath, float px, float py,
		float pz, float rx, float ry, float rz, float sx, float sy, float sz)
	{
		auto begin = std::chrono::high_resolution_clock::now();
		VkMeshPtr object = LoadModelIfNotExist(mesh, texturePath);
		glm::mat4 modelMatrix = glm::mat4(1.0f);
		modelMatrix = glm::translate(modelMatrix, glm::vec3(px, py, pz));
		modelMatrix = glm::rotate(modelMatrix, glm::radians(rx), glm::vec3(1.0f, 0.0f, 0.0f));
		modelMatrix = glm::rotate(modelMatrix, glm::radians(ry), glm::vec3(0.0f, 1.0f, 0.0f));
		modelMatrix = glm::rotate(modelMatrix, glm::radians(rz), glm::vec3(0.0f, 0.0f, 1.0f));
		modelMatrix = glm::scale(modelMatrix, glm::vec3(sx, sy, sz));
		if (_renderObjectsToDraw.find(object) == _renderObjectsToDraw.end())
		{
			auto end = std::chrono::high_resolution_clock::now();
			auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - begin);
			_drawObjectsTime = std::chrono::duration<float>(end - begin).count();
			_renderObjectsToDraw.emplace(object, std::vector<glm::mat4>{ modelMatrix });
			return;
		}
		_renderObjectsToDraw[object].push_back(modelMatrix);
		auto end = std::chrono::high_resolution_clock::now();
		auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - begin);
		_drawObjectsTime = std::chrono::duration<float>(end - begin).count();
	}

	VkMeshPtr VulkanRenderer::LoadModelIfNotExist(MeshPtr& mesh, const std::string& texturePath)
	{
		auto it = _meshes.find(mesh->GetPath());
		if (it != _meshes.end())
			return it->second;
		VkMeshPtr vkMesh = std::make_shared<VkMesh>();
		for (SubMeshPtr& subMesh : mesh->GetSubMeshes())
		{
			VkSubMeshPtr vkSubMesh = std::make_shared<VkSubMesh>(subMesh, *_allocator,
				VK_BUFFER_USAGE_VERTEX_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT,
				VMA_MEMORY_USAGE_GPU_ONLY);
			vkSubMesh->Upload(_uploadContext->_commandBuffer,
				_uploadContext->_commandPool,
				_uploadContext->_uploadFence,
				*_graphicsQueue,
				*_allocator);
			vkMesh->subMeshes.push_back(vkSubMesh);
			//TODO: Add support for all material properties
			VkPipelineLayout pipelineLayout = *_meshPipelineLayout->Get();
			VkPipeline pipeline = *_coloredShaderPipeline->Get();
			if (!subMesh->GetMaterial()->diffuseTexturePath.empty())
			{
				pipelineLayout = *_texturedSetLayout->Get();
				pipeline = *_texturedPipeline->Get();
			}
			if (!subMesh->GetMaterial()->diffuseTexturePath.empty())
			{
				std::filesystem::path path = mesh->GetPath();
				path = path.parent_path() / subMesh->GetMaterial()->diffuseTexturePath;
				subMesh->GetMaterial()->diffuseTexture = _textureBuilder->BuildTexture(path.string());
				_materialBuilder->BuildMaterial(*subMesh->GetMaterial(), pipelineLayout, pipeline);
			}
			else
			{
				_materialBuilder->BuildMaterial(*subMesh->GetMaterial(), pipelineLayout, pipeline);
			}
			_meshes.emplace(mesh->GetPath(), vkMesh);
		}
		return vkMesh;
	}

	void VulkanRenderer::DrawObjects(const Camera& camera)
	{
		std::uint32_t windowWidth = _window.GetWidth();
		std::uint32_t windowHeight = _window.GetHeight();
		VkViewport viewport{
			.width = static_cast<float>(windowWidth),
			.height = static_cast<float>(windowHeight),
			.minDepth = 0.0f,
			.maxDepth = 1.0f,
		};
		VkRect2D dynamicScissor = {};
		dynamicScissor.offset = { 0, 0 };
		dynamicScissor.extent = { windowWidth, windowHeight };
		FrameData& frame = _frames[_frameNumber % _frames.size()];
		auto minimumAlignment = _gpuProperties.limits.minUniformBufferOffsetAlignment;

		auto beginTime = std::chrono::high_resolution_clock::now();
		MapAndCopy(*_allocator, frame._cameraBuffer, camera);

		int frameIndex = _frameNumber % 2;
		MapAndCopy(*_allocator, *_sceneParameterBuffer, _sceneParameters,
			PadUniformBuffer(sizeof(GPUSceneData), minimumAlignment) * frameIndex);

		for (auto [object, modelMatrices] : _renderObjectsToDraw)
		{
			MapAndCopy<GPUObjectData, glm::mat4>(*_allocator, frame._objectBuffer, modelMatrices,
				[](GPUObjectData& gpuObjectData, glm::mat4& modelMatrix)
				{
				  gpuObjectData.modelMatrix = modelMatrix;
				});
		}

		std::size_t totalObjectCount = 0;
		for (auto& [object, modelMatrices] : _renderObjectsToDraw)
		{
			std::size_t total = 0;
			for (auto& subMesh : object->subMeshes)
			{
				total++;
			}
			totalObjectCount += total * modelMatrices.size();
		}

		auto* drawIndirectCommand = MapBuffer<VkDrawIndirectCommand>(frame._indirectBuffer);

		std::size_t i = 0;
		for (auto& [object, modelMatrices] : _renderObjectsToDraw)
		{
			for (auto& subMesh : object->subMeshes)
			{
				for (std::size_t j = 0; j < modelMatrices.size(); ++j)
				{
					VkDrawIndirectCommand* drawIndirectCommandPtr = &drawIndirectCommand[i];
					drawIndirectCommandPtr->vertexCount = subMesh->GetVertices().size();
					drawIndirectCommandPtr->instanceCount = 1;
					drawIndirectCommandPtr->firstVertex = 0;
					drawIndirectCommandPtr->firstInstance = j;
					i++;
				}
			}
		}
		UnMapBuffer(frame._indirectBuffer);
		auto endTime = std::chrono::high_resolution_clock::now();
		auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(endTime - beginTime);
		_transfersTime = std::chrono::duration<float>(endTime - beginTime).count();
		beginTime = std::chrono::high_resolution_clock::now();
		VkSubMeshPtr lastSubMesh = nullptr;
		beginTime = std::chrono::high_resolution_clock::now();
		i = 0;
		for (auto& [object, modelMatrices] : _renderObjectsToDraw)
		{
			for (auto& subMesh : object->subMeshes)
			{
				for (std::size_t j = 0; j < modelMatrices.size(); ++j)
				{
					auto vkMaterial = _materialBuilder->GetMaterial(subMesh->GetMaterial()->name);
					std::uint32_t
						uniform_offset = PadUniformBuffer(sizeof(GPUSceneData), minimumAlignment) * frameIndex;
					frame._mainCommandBuffer->BindPipeline(VK_PIPELINE_BIND_POINT_GRAPHICS, vkMaterial->_pipeline);
					frame._mainCommandBuffer->BindDescriptorSets(VK_PIPELINE_BIND_POINT_GRAPHICS,
						vkMaterial->_pipelineLayout, 0, 1, frame.globalDescriptor, uniform_offset);
					frame._mainCommandBuffer->BindDescriptorSets(VK_PIPELINE_BIND_POINT_GRAPHICS,
						vkMaterial->_pipelineLayout, 1, 1, frame.objectDescriptor);
					if (vkMaterial->_diffuseTextureSet && !vkMaterial->_diffuseTextureSet->IsNull())
					{
						frame._mainCommandBuffer->BindDescriptorSets(VK_PIPELINE_BIND_POINT_GRAPHICS,
							vkMaterial->_pipelineLayout,
							2, 1,
							*vkMaterial->_diffuseTextureSet);
					}

					if (subMesh != lastSubMesh)
					{
						frame._mainCommandBuffer->BindVertexBuffers(subMesh->_vertexBuffer);
						lastSubMesh = subMesh;
					}
					if (frame._isResized)
					{
						frame._mainCommandBuffer->SetViewport(viewport);
						frame._mainCommandBuffer->SetScissor(dynamicScissor);
					}
					frame._mainCommandBuffer->DrawIndirect(frame._indirectBuffer,
						sizeof(VkDrawIndirectCommand) * i,
						drawIndirectCommand[i].instanceCount,
						sizeof(VkDrawIndirectCommand));
					++i;
				}
			}
		}
		endTime = std::chrono::high_resolution_clock::now();
		duration = std::chrono::duration_cast<std::chrono::milliseconds>(endTime - beginTime);
		_drawTime = std::chrono::duration<float>(endTime - beginTime).count();
	}

	void VulkanRenderer::UseImGUI()
	{
		assert(!_imGUI);
		RenderingContext context = {
			.instance = &_vulkanInstance,
			.physicalDevice = &_physicalDevice,
			.device = &_device,
			.queueFamilyIndex = _graphicsQueueFamilyIndex,
			.queue = _graphicsQueue.get(),
			.renderPass = _renderPass.get(),
			.minImageCount = 3,
			.imageCount = 3,
			.commandBuffer = &_uploadContext->_commandBuffer,
			.fence = &_uploadContext->_uploadFence,
			.commandPool = &_uploadContext->_commandPool,
		};
		_imGUI = std::make_unique<ImGUI>(context, _window);
	}

	ImGUI* VulkanRenderer::GetImGUIContext()
	{
		if (_imGUI)
			return _imGUI.get();
		return nullptr;
	}

	void VulkanRenderer::UpdateSceneParameters(const Scene& sceneData)
	{
		_sceneParameters = sceneData;
	}

	void VulkanRenderer::Resize(std::uint32_t width, std::uint32_t height)
	{
		_device.WaitIdle();
		_frameBuffers.clear();
		_swapchain.reset();
		_swapchain = std::make_unique<Swapchain>(_device, *_allocator, VkExtent2D{ width, height }, _physicalDevice);
		auto swapchainImagesViews = _swapchain->GetImageViews();
		auto& swapchainDepthImageView = _swapchain->GetDepthImageView();
		for (auto& swapchainImagesView : swapchainImagesViews)
		{
			_frameBuffers.emplace_back(_device,
				*_renderPass,
				swapchainImagesView,
				swapchainDepthImageView,
				VkExtent2D{ width, height });
		}
		for (auto& frame : _frames)
		{
			frame._isResized = true;
		}
		_renderInfo.width = width;
		_renderInfo.height = height;
	}
} // Concerto::Graphics