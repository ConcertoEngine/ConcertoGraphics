//
// Created by arthur on 25/10/2022.
//

#define GLFW_INCLUDE_VULKAN
#include <cassert>
#include <vector>
#include <iostream>

#include "imgui.h"
#include <imgui_impl_vulkan.h>
#include "imgui_impl_glfw.h"
#include <GLFW/glfw3.h>
#include "glm/glm.hpp"

#include "MeshPushConstants.hpp"
#include "Utils.hpp"
#include "VulkanRenderer.hpp"
#include "window/GlfW3.hpp"
#include "wrapper/Sampler.hpp"

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
		_allocator = std::move(Allocator(_physicalDevice, _device, _vulkanInstance));
		_swapchain = std::move(
				Swapchain(_device, _allocator.value(), { _window.GetWidth(), _window.GetHeight() }, _physicalDevice));
		_renderPass = std::move(RenderPass(_device, _swapchain.value()));
		auto swapchainImagesViews = _swapchain.value().GetImageViews();
		auto& swapchainDepthImageView = _swapchain.value().GetDepthImageView();
		for (auto& swapchainImagesView: swapchainImagesViews)
		{
			_frameBuffers.emplace_back(std::move(
					FrameBuffer(_device, _renderPass.value(), swapchainImagesView, swapchainDepthImageView,
							{ _window.GetWidth(), _window.GetHeight() })));
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
		_globalSetLayout = std::move(DescriptorSetLayout(_device, { camBufferBind, sceneBind }));
		_objectSetLayout = std::move(DescriptorSetLayout(_device, { objectBind }));
		_singleTextureSetLayout = std::move(DescriptorSetLayout(_device, { textureBind }));
		_descriptorPool = std::move(DescriptorPool(_device));
		const std::size_t sceneParamBufferSize =
				2 * PadUniformBuffer(sizeof(GPUSceneData), _gpuProperties.limits.minUniformBufferOffsetAlignment);
		_sceneParameterBuffer = std::move(
				AllocatedBuffer(*_allocator, sceneParamBufferSize, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT,
						VMA_MEMORY_USAGE_CPU_TO_GPU));
		_frames.reserve(_swapchain.value().GetImages().size());
		for (std::size_t i = 0; i < _swapchain.value().GetImages().size(); i++)
		{
			_frames.emplace_back(std::move(
					FrameData(_device, _allocator.value(), _graphicsQueueFamilyIndex, *_descriptorPool,
							*_globalSetLayout, *_objectSetLayout, *_sceneParameterBuffer, true)));
		}
		// Commands
		_colorMeshShader = std::move(ShaderModule(_device, R"(shaders/default_lit.frag.spv)"));
		_textureMeshShader = std::move(ShaderModule(_device, R"(shaders/textured_lit.frag.spv)"));
		_meshVertShader = std::move(ShaderModule(_device, R"(shaders/tri_mesh_ssbo.vert.spv)"));
		_meshPipelineLayout = std::move(makePipelineLayout<MeshPushConstants>(_device,
				{ *_globalSetLayout, *_objectSetLayout }));
		_texturedSetLayout = std::move(makePipelineLayout<MeshPushConstants>(_device,
				{ *_globalSetLayout, *_objectSetLayout, *_singleTextureSetLayout }));
		PipelineInfo meshPipelineInfo({ VulkanInitializer::PipelineShaderStageCreateInfo(VK_SHADER_STAGE_VERTEX_BIT,
				*_meshVertShader->Get()), VulkanInitializer::PipelineShaderStageCreateInfo(VK_SHADER_STAGE_FRAGMENT_BIT,
				*_colorMeshShader->Get()) }, { _window.GetWidth(), _window.GetHeight() }, *_meshPipelineLayout);
		_coloredShaderPipeline = std::move(Pipeline(_device, meshPipelineInfo));
		_coloredShaderPipeline->BuildPipeline(*_renderPass->Get());
		meshPipelineInfo._shaderStages.clear();
		meshPipelineInfo._shaderStages.push_back(
				VulkanInitializer::PipelineShaderStageCreateInfo(VK_SHADER_STAGE_VERTEX_BIT,
						*_meshVertShader->Get()));
		meshPipelineInfo._shaderStages.push_back(
				VulkanInitializer::PipelineShaderStageCreateInfo(VK_SHADER_STAGE_FRAGMENT_BIT,
						*_textureMeshShader->Get()));
		meshPipelineInfo._pipelineLayout = *_texturedSetLayout->Get();
		_texturedPipeline = std::move(Pipeline(_device, meshPipelineInfo));
		_texturedPipeline->BuildPipeline(*_renderPass->Get());
		_graphicsQueue = std::move(Queue(_device, _graphicsQueueFamilyIndex));
		_uploadContext = std::move(UploadContext(_device, _graphicsQueueFamilyIndex));
		if (_renderInfo.useImGUI)
			UseImGUI();
	}

	VulkanRenderer* VulkanRenderer::Instance()
	{
		assert(_instance != nullptr);
		return _instance;
	}

	void VulkanRenderer::Draw(const Camera &camera)
	{
		if (_imGUI.has_value())
			_imGUI.value().Draw();
		FrameData& frame = _frames[_frameNumber % _frames.size()];
		frame._renderFence.wait(1000000000);
		frame._renderFence.reset();
		std::uint32_t swapchainImageIndex = _swapchain->AcquireNextImage(frame._presentSemaphore, frame._renderFence,
				1000000000);
		frame._mainCommandBuffer->Reset();
		frame._mainCommandBuffer->Begin();
		{
			VkClearValue clearValue;
			VkClearValue depthClear;
			clearValue.color = {{ 0.0f, 0.0f, 255, 1.0f }};
			depthClear.depthStencil.depth = 1.f;
			VkClearValue clearValues[] = { clearValue, depthClear };
			VkRenderPassBeginInfo rpInfo = VulkanInitializer::RenderPassBeginInfo(*_renderPass->Get(),
					{ _window.GetWidth(), _window.GetHeight() }, *_frameBuffers[swapchainImageIndex].Get());
			rpInfo.clearValueCount = 2;
			rpInfo.pClearValues = &clearValues[0];
			frame._mainCommandBuffer->BeginRenderPass(rpInfo);
			{
				DrawObjects(camera);
				if(_imGUI.has_value())
					_imGUI.value().RenderDrawData(*frame._mainCommandBuffer);
			}
			frame._mainCommandBuffer->EndRenderPass();
		}
		frame._mainCommandBuffer->End();

		_graphicsQueue->Submit(frame);
		_graphicsQueue->Present(frame, *_swapchain, swapchainImageIndex);
		_frameNumber++;
		_renderObjectsToDraw.clear();
	}

	void VulkanRenderer::DrawObject(const std::string& modelPath, const std::string& texturePath, float px, float py,
			float pz, float rx, float ry, float rz, float sx, float sy, float sz)
	{
		RenderObject& object = LoadModelIfNotExist(modelPath, texturePath, px, py, pz, rx, ry, rz, sx, sy, sz);
		_renderObjectsToDraw.emplace_back(object);
	}

	RenderObject&
	VulkanRenderer::LoadModelIfNotExist(const std::string& modelPath, const std::string& texturePath, float px,
			float py,
			float pz, float rx, float ry, float rz, float sx, float sy, float sz)
	{
		auto it = _renderObjects.find(modelPath);
		if (it != _renderObjects.end())
			return *it->second;
		std::unique_ptr<Mesh> mesh = std::make_unique<Mesh>(modelPath, *_allocator,
				VK_BUFFER_USAGE_VERTEX_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT,
				VMA_MEMORY_USAGE_GPU_ONLY);
		mesh->Upload(_uploadContext->_commandBuffer, _uploadContext->_commandPool, _uploadContext->_uploadFence,
				*_graphicsQueue, *_allocator);
		VkPipelineLayout pipelineLayout = *_meshPipelineLayout->Get();
		VkPipeline pipeline = *_coloredShaderPipeline->Get();
		if (!texturePath.empty())
		{
			pipelineLayout = *_texturedSetLayout->Get();
			pipeline = *_texturedPipeline->Get();
		}
		glm::mat4 modelMatrix = glm::mat4(1.0f);
//		modelMatrix = glm::translate(modelMatrix, glm::vec3(px, py, pz));
//		modelMatrix = glm::rotate(modelMatrix, glm::radians(0.f), glm::vec3(rx, ry, rz));
//		modelMatrix = glm::scale(modelMatrix, glm::vec3(sx, sy, sz));

		auto& renderObject = _renderObjects.emplace(modelPath,
				std::make_unique<RenderObject>(std::move(mesh), pipelineLayout, pipeline, modelMatrix)).first->second;
		if (!texturePath.empty())
		{
			Texture& texture = CreateTextureIfNotExist(texturePath);
			Sampler sampler(_device, VK_FILTER_NEAREST);
			renderObject->material._textureSet = _descriptorPool->AllocateDescriptorSet(*_singleTextureSetLayout);
			renderObject->material._textureSet->WriteImageSamplerDescriptor(sampler, texture._imageView);
		}
		return *renderObject;
	}

	Texture& VulkanRenderer::CreateTextureIfNotExist(const std::string& texturePath)
	{
		auto it = _textures.find(texturePath);
		if (it != _textures.end())
			return *it->second;
		auto texture = std::make_shared<Texture>(_device, texturePath, *_allocator,
				_uploadContext->_commandBuffer, *_uploadContext, *_graphicsQueue, VK_IMAGE_ASPECT_COLOR_BIT);
		return *_textures.emplace(texturePath, texture).first->second;
	}

	void VulkanRenderer::DrawObjects(const Camera &camera)
	{
		FrameData& frame = _frames[_frameNumber % _frames.size()];
		auto minimumAlignment = _gpuProperties.limits.minUniformBufferOffsetAlignment;

		Mesh* lastMesh = nullptr;
		Material* lastMaterial = nullptr;

		static GPUSceneData _sceneParameters;
		MapAndCopy(*_allocator, frame._cameraBuffer, camera);

		_sceneParameters.ambientColor = { 255, 0, 0, 0 };

		int frameIndex = _frameNumber % 2;
		MapAndCopy(*_allocator, *_sceneParameterBuffer, _sceneParameters,
				PadUniformBuffer(sizeof(GPUSceneData), minimumAlignment) * frameIndex);
		MapAndCopy<GPUObjectData, std::reference_wrapper<RenderObject>>(*_allocator, frame._objectBuffer,
				_renderObjectsToDraw,
				[](GPUObjectData& gpuObjectData, std::reference_wrapper<RenderObject>& renderObject)
				{
					gpuObjectData.modelMatrix = renderObject.get().transformMatrix;
				});
		for (std::size_t i = 0; i < _renderObjectsToDraw.size(); i++)
		{
			RenderObject& object = _renderObjectsToDraw[i];

			if (lastMaterial == nullptr || object.material != *lastMaterial)
			{
				std::uint32_t uniform_offset = PadUniformBuffer(sizeof(GPUSceneData), minimumAlignment) * frameIndex;
				frame._mainCommandBuffer->BindPipeline(VK_PIPELINE_BIND_POINT_GRAPHICS, object.material._pipeline);
				lastMaterial = &object.material;
				frame._mainCommandBuffer->BindDescriptorSets(VK_PIPELINE_BIND_POINT_GRAPHICS,
						object.material._pipelineLayout, 0, 1,
						frame.globalDescriptor, uniform_offset);
				frame._mainCommandBuffer->BindDescriptorSets(VK_PIPELINE_BIND_POINT_GRAPHICS,
						object.material._pipelineLayout, 1, 1,
						frame.objectDescriptor);
				if (object.material._textureSet.has_value() && !object.material._textureSet->IsNull())
				{
					frame._mainCommandBuffer->BindDescriptorSets(VK_PIPELINE_BIND_POINT_GRAPHICS,
							object.material._pipelineLayout,
							2, 1,
							*object.material._textureSet);
				}
			}

			if (object.mesh.get() != lastMesh)
			{
				frame._mainCommandBuffer->BindVertexBuffers(object.mesh->_vertexBuffer);
				lastMesh = object.mesh.get();
			}
			frame._mainCommandBuffer->Draw(object.mesh->_vertices.size(), 1, 0, i);
		}
	}

	void VulkanRenderer::UseImGUI()
	{
		assert(_imGUI.has_value() == false);
		RenderingContext context  = {
			.instance = &_vulkanInstance,
			.physicalDevice = &_physicalDevice,
			.device = &_device,
			.queueFamilyIndex = _graphicsQueueFamilyIndex,
			.queue = &(_graphicsQueue.value()),
			.renderPass = &(_renderPass.value()),
			.minImageCount = 3,
			.imageCount = 3,
			.commandBuffer = &_uploadContext->_commandBuffer,
			.fence = &_uploadContext->_uploadFence,
			.commandPool = &_uploadContext->_commandPool,
		};
		_imGUI.emplace(context, _window);
	}

	ImGUI* VulkanRenderer::GetImGUIContext()
	{
		if (_imGUI.has_value())
			return &(_imGUI.value());
		return nullptr;
	}
} // Concerto::Graphics