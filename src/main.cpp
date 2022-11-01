//
// Created by arthur on 12/07/2022.
//
#define VMA_IMPLEMENTATION
#define VMA_VULKAN_VERSION 1000000
#define VKB_DEBUG

#include <iostream>
#include <unordered_map>
#include <array>
#include "glm/gtx/transform.hpp"
#include "VulkanRenderer.hpp"
#include "MeshPushConstants.hpp"
#include "Vertex.hpp"
#include "vulkan/vulkan.h"
#include "window/GlfW3.hpp"
#include "wrapper/Device.hpp"
#include "wrapper/Swapchain.hpp"
#include "wrapper/RenderPass.hpp"
#include "wrapper/FrameBuffer.hpp"
#include "wrapper/CommandBuffer.hpp"
#include "wrapper/DescriptorSetLayout.hpp"
#include "wrapper/DescriptorPool.hpp"
#include "wrapper/AllocatedBuffer.hpp"
#include "wrapper/VulkanInitializer.hpp"
#include "wrapper/Allocator.hpp"
#include "wrapper/ShaderModule.hpp"
#include "wrapper/Pipeline.hpp"
#include "wrapper/PipelineInfo.hpp"
#include "wrapper/PipelineLayout.hpp"
#include "wrapper/Queue.hpp"
#include "wrapper/Sampler.hpp"
#include "wrapper/Instance.hpp"
#include "wrapper/PhysicalDevice.hpp"
#include "Mesh.hpp"
#include "Frame.hpp"
#include "Material.hpp"
#include "RenderObject.hpp"
#include "GPUData.hpp"
#include "Utils.hpp"
#include "UploadContext.hpp"
#include "Texture.hpp"

using namespace Concerto;
using namespace Concerto::Graphics;
using namespace Concerto::Graphics::Wrapper;

VkInstance _instance{ VK_NULL_HANDLE };
VkDevice _device{ VK_NULL_HANDLE };
int _frameNumber = 0;
VkExtent2D windowExtent = { 1280, 720 };
VkPhysicalDeviceProperties _gpuProperties{};


using Frames = std::array<FrameData, 2>;
std::vector<std::unique_ptr<RenderObject>> _renderables;
std::unordered_map<std::string, std::shared_ptr<Texture>> textures;

void drawObjects(CommandBuffer& commandBuffer, AllocatedBuffer& sceneParameterBuffer);

void
draw(Allocator& allocator, Swapchain& swapchain, RenderPass& renderpass, FrameBuffer& frameBuffer, FrameData& frame,
		AllocatedBuffer& sceneParameterBuffer, Queue& queue);


int main()
{
#if defined(_WIN32)
	std::vector<const char*> extensions = { "VK_KHR_surface", "VK_KHR_win32_surface" };
#elif defined(__linux__)
	std::vector<const char*> extensions = { "VK_KHR_surface", "VK_KHR_xcb_surface", "VK_KHR_xlib_surface", "VK_KHR_wayland_surface" };
#endif
	const char* appName = "Concerto";
	std::vector<const char*> layers = { "VK_LAYER_KHRONOS_validation" /*, "VK_LAYER_LUNARG_api_dump"*/};
	IWindowPtr window = std::make_unique<GlfW3>(appName, windowExtent.width, windowExtent.height);
	Instance instance("Concerto", "ConcertoVulkan", { 1, 1, 0 }, { 1, 0, 0, }, { 1, 0, 0 }, extensions, layers);
	VkSurfaceKHR vkSurface{ VK_NULL_HANDLE };
	auto res = glfwCreateWindowSurface(*instance.Get(), (GLFWwindow*)window->getRawWindow(), nullptr, &vkSurface);
	std::span<PhysicalDevice> devices = instance.EnumeratePhysicalDevices();
	if (devices.empty())
	{
		std::cerr << "No GPU found" << std::endl;
		return -1;
	}
	PhysicalDevice& physicalDevice = devices[0];
	physicalDevice.SetSurface(vkSurface);
	std::vector<const char*> deviceExtensions = { VK_KHR_SWAPCHAIN_EXTENSION_NAME };
	Device device(physicalDevice,deviceExtensions);

	if (res != VK_SUCCESS)
	{
		std::cerr << "Failed to create window surface code : " << res << std::endl;
		return -1;
	}

	_gpuProperties = physicalDevice.GetProperties();
	VkPhysicalDevice _physicalDevice = *physicalDevice.Get();
	VkQueue _graphicsQueue = *device.GetQueue(Queue::Type::Graphics).Get();
	_device = *device.Get();
	_instance = *instance.Get();
	uint32_t _graphicsQueueFamily = device.GetQueue(Queue::Type::Graphics).GetFamilyIndex();
	Allocator _allocator(_physicalDevice, _device, _instance);
	Swapchain swapchain(_allocator, windowExtent, physicalDevice, device, _instance);

	// Renderpass


	RenderPass renderPass(_device, swapchain);
	// Renderpass
	FrameBuffer frameBuffer(_device, swapchain, renderPass);
	// Commands
	VkDescriptorSetLayoutBinding camBufferBind = VulkanInitializer::DescriptorSetLayoutBinding(
			VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, VK_SHADER_STAGE_VERTEX_BIT, 0);
	VkDescriptorSetLayoutBinding sceneBind = VulkanInitializer::DescriptorSetLayoutBinding(
			VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC, VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT, 1);
	VkDescriptorSetLayoutBinding objectBind = VulkanInitializer::DescriptorSetLayoutBinding(
			VK_DESCRIPTOR_TYPE_STORAGE_BUFFER, VK_SHADER_STAGE_VERTEX_BIT, 0);
	VkDescriptorSetLayoutBinding textureBind = VulkanInitializer::DescriptorSetLayoutBinding(
			VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, VK_SHADER_STAGE_FRAGMENT_BIT, 0);
	DescriptorSetLayout globalSetLayout(_device, { camBufferBind, sceneBind });
	DescriptorSetLayout objectSetLayout(_device, { objectBind });
	DescriptorSetLayout singleTextureSetLayout(_device, { textureBind });

	std::vector<VkDescriptorPoolSize> sizes =
			{
					{ VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,         10 },
					{ VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC, 10 },
					{ VK_DESCRIPTOR_TYPE_STORAGE_BUFFER,         10 },
					{ VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 10 }
			};
	DescriptorPool descriptorPool(_device, sizes);
	const std::size_t sceneParamBufferSize =
			2 * PadUniformBuffer(sizeof(GPUSceneData), _gpuProperties.limits.minUniformBufferOffsetAlignment);
	AllocatedBuffer _sceneParameterBuffer(_allocator, sceneParamBufferSize, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT,
			VMA_MEMORY_USAGE_CPU_TO_GPU);
	Frames frames = {
			FrameData(_allocator, _device, _graphicsQueueFamily, descriptorPool, globalSetLayout, objectSetLayout,
					_sceneParameterBuffer, true),
			FrameData(_allocator, _device, _graphicsQueueFamily, descriptorPool, globalSetLayout, objectSetLayout,
					_sceneParameterBuffer, true)
	};
	// Commands
	// Pilpeline
	ShaderModule colorMeshShader(R"(shaders/default_lit.frag.spv)", _device);
	ShaderModule texturedMeshShader(R"(shaders/textured_lit.frag.spv)", _device);
	ShaderModule meshVertShader(R"(shaders/tri_mesh_ssbo.vert.spv)", _device);

	PipelineLayout meshPipelineLayout = makePipelineLayout<MeshPushConstants>(_device,
			{ globalSetLayout, objectSetLayout });
	PipelineLayout texturedSetLayout = makePipelineLayout<MeshPushConstants>(_device,
			{ globalSetLayout, objectSetLayout, singleTextureSetLayout });

	PipelineInfo meshPipelineInfo;

	meshPipelineInfo._shaderStages.push_back(
			VulkanInitializer::PipelineShaderStageCreateInfo(VK_SHADER_STAGE_VERTEX_BIT,
					meshVertShader.getShaderModule()));
	meshPipelineInfo._shaderStages.push_back(
			VulkanInitializer::PipelineShaderStageCreateInfo(VK_SHADER_STAGE_FRAGMENT_BIT,
					colorMeshShader.getShaderModule()));
	VertexInputDescription vertexDescription = Vertex::getVertexDescription();
	meshPipelineInfo._vertexInputInfo = VulkanInitializer::VertexInputStateCreateInfo();
	meshPipelineInfo._vertexInputInfo.pVertexAttributeDescriptions = vertexDescription.attributes.data();
	meshPipelineInfo._vertexInputInfo.vertexAttributeDescriptionCount = vertexDescription.attributes.size();
	meshPipelineInfo._vertexInputInfo.pVertexBindingDescriptions = vertexDescription.bindings.data();
	meshPipelineInfo._vertexInputInfo.vertexBindingDescriptionCount = vertexDescription.bindings.size();
	meshPipelineInfo._inputAssembly = VulkanInitializer::InputAssemblyCreateInfo(VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST);
	meshPipelineInfo._viewport.x = 0.0f;
	meshPipelineInfo._viewport.y = 0.0f;
	meshPipelineInfo._viewport.width = (float)windowExtent.width;
	meshPipelineInfo._viewport.height = (float)windowExtent.height;
	meshPipelineInfo._viewport.minDepth = 0.0f;
	meshPipelineInfo._viewport.maxDepth = 1.0f;
	meshPipelineInfo._scissor.offset = { 0, 0 };
	meshPipelineInfo._scissor.extent = windowExtent;
	meshPipelineInfo._rasterizer = VulkanInitializer::RasterizationStateCreateInfo(VK_POLYGON_MODE_FILL);
	meshPipelineInfo._multisampling = VulkanInitializer::MultisamplingStateCreateInfo();
	meshPipelineInfo._colorBlendAttachment = VulkanInitializer::ColorBlendAttachmentState();
	meshPipelineInfo._pipelineLayout = meshPipelineLayout.Get();
	meshPipelineInfo._depthStencil = VulkanInitializer::DepthStencilCreateInfo(true, true, VK_COMPARE_OP_LESS_OR_EQUAL);
	Pipeline coloredShaderPipeline(_device, meshPipelineInfo);
	coloredShaderPipeline.buildPipeline(renderPass.Get());
	meshPipelineInfo._shaderStages.clear();
	meshPipelineInfo._shaderStages.push_back(
			VulkanInitializer::PipelineShaderStageCreateInfo(VK_SHADER_STAGE_VERTEX_BIT,
					meshVertShader.getShaderModule()));
	meshPipelineInfo._shaderStages.push_back(
			VulkanInitializer::PipelineShaderStageCreateInfo(VK_SHADER_STAGE_FRAGMENT_BIT,
					texturedMeshShader.getShaderModule()));
	meshPipelineInfo._pipelineLayout = texturedSetLayout.Get();
	Pipeline texturedPipeline(_device, meshPipelineInfo);
	texturedPipeline.buildPipeline(renderPass.Get());

	Queue queue(device, _graphicsQueueFamily);
	UploadContext uploadContext(_device, _graphicsQueueFamily);
//	Fence _renderFence(_device);
	auto texture = std::make_shared<Texture>(".\\assets\\lost_empire-RGBA.png", _allocator,
			uploadContext._commandBuffer, uploadContext, queue, VK_IMAGE_ASPECT_COLOR_BIT, _device);
	textures.emplace("empire_diffuse", texture);
	// Monkey Mesh
	{
		std::unique_ptr<Mesh> monkeyMesh = std::make_unique<Mesh>(".\\assets\\monkey_flat.obj", _allocator,
				VK_BUFFER_USAGE_VERTEX_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT,
				VMA_MEMORY_USAGE_GPU_ONLY);
		monkeyMesh->Upload(uploadContext._commandBuffer, uploadContext._commandPool, uploadContext._uploadFence, queue,
				_allocator);
		auto& renderObj = _renderables.emplace_back(
				std::make_unique<RenderObject>(std::move(monkeyMesh), meshPipelineLayout.Get(),
						coloredShaderPipeline.Get(),
						glm::mat4{ 1.0f }));
	}
	// Lost empire
	{
		std::unique_ptr<Mesh> empireMesh = std::make_unique<Mesh>(".\\assets\\lost_empire.obj", _allocator,
				VK_BUFFER_USAGE_VERTEX_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT,
				VMA_MEMORY_USAGE_GPU_ONLY);
		empireMesh->Upload(uploadContext._commandBuffer, uploadContext._commandPool, uploadContext._uploadFence, queue,
				_allocator);
		auto& renderObj = _renderables.emplace_back(
				std::make_unique<RenderObject>(std::move(empireMesh), texturedSetLayout.Get(), texturedPipeline.Get(),
						glm::mat4{ 1.0f }));
		Sampler sampler(_device, VK_FILTER_NEAREST);
		renderObj->material._textureSet = descriptorPool.AllocateDescriptorSet(singleTextureSetLayout);
		renderObj->material._textureSet.WriteImageSamplerDescriptor(sampler, texture->_imageView);
	}

	// Render loop
	while (true)
	{
		window->popEvent();
		draw(_allocator, swapchain, renderPass, frameBuffer, frames[_frameNumber % frames.size()],
				_sceneParameterBuffer, queue);
	}
	// Render loop
}

void
drawObjects(Allocator& allocator, CommandBuffer& commandBuffer, FrameData& frame, AllocatedBuffer& sceneParameterBuffer)
{
	auto minimumAlignment = _gpuProperties.limits.minUniformBufferOffsetAlignment;
	glm::vec3 camPos = { 0.f, -6.f, -10.f };

	glm::mat4 view = glm::translate(glm::mat4(1.f), camPos);
	glm::mat4 projection = glm::perspective(glm::radians(70.f), 1700.f / 900.f, 0.1f, 200.0f);
	projection[1][1] *= -1;

	Mesh* lastMesh = nullptr;
	Material* lastMaterial = nullptr;

	GPUCameraData camData{};
	camData.proj = projection;
	camData.view = view;
	camData.viewproj = projection * view;
	static GPUSceneData _sceneParameters;

	MapAndCopy(allocator, frame._cameraBuffer, camData);

	_sceneParameters.ambientColor = { 255, 0, 255, 1 };

	int frameIndex = _frameNumber % 2;
	MapAndCopy(allocator, sceneParameterBuffer, _sceneParameters,
			PadUniformBuffer(sizeof(GPUSceneData), minimumAlignment) * frameIndex);
	MapAndCopy<GPUObjectData, std::unique_ptr<RenderObject>>(allocator, frame._objectBuffer, _renderables,
			[](GPUObjectData& gpuObjectData, std::unique_ptr<RenderObject>& renderObject)
			{
				gpuObjectData.modelMatrix = renderObject->transformMatrix;
			});
	for (std::size_t i = 0; i < _renderables.size(); i++)
	{
		RenderObject& object = *_renderables[i];

		if (lastMaterial == nullptr || object.material != *lastMaterial)
		{
			std::uint32_t uniform_offset = PadUniformBuffer(sizeof(GPUSceneData), minimumAlignment) * frameIndex;
			commandBuffer.BindPipeline(VK_PIPELINE_BIND_POINT_GRAPHICS, object.material._pipeline);
			lastMaterial = &object.material;
			commandBuffer.BindDescriptorSets(VK_PIPELINE_BIND_POINT_GRAPHICS, object.material._pipelineLayout, 0, 1,
					frame.globalDescriptor, uniform_offset);
			commandBuffer.BindDescriptorSets(VK_PIPELINE_BIND_POINT_GRAPHICS, object.material._pipelineLayout, 1, 1,
					frame.objectDescriptor);
			if (*object.material._textureSet.Get() != VK_NULL_HANDLE)
			{
				commandBuffer.BindDescriptorSets(VK_PIPELINE_BIND_POINT_GRAPHICS, object.material._pipelineLayout, 2, 1,
						object.material._textureSet);
			}
		}
		glm::mat4 mesh_matrix = object.transformMatrix;

		if (object.mesh.get() != lastMesh)
		{
			commandBuffer.BindVertexBuffers(object.mesh->_vertexBuffer);
			lastMesh = object.mesh.get();
		}
		commandBuffer.Draw(object.mesh->_vertices.size(), 1, 0, i);
	}
}

void
draw(Allocator& allocator, Swapchain& swapchain, RenderPass& renderpass, FrameBuffer& frameBuffer, FrameData& frame,
		AllocatedBuffer& sceneParameterBuffer, Queue& queue)
{
	frame._renderFence.wait(1000000000);
	frame._renderFence.reset();
	std::uint32_t swapchainImageIndex = swapchain.AcquireNextImage(frame._presentSemaphore, frame._renderFence,
			1000000000);
	frame._mainCommandBuffer->Reset();
	frame._mainCommandBuffer->Begin();
	{
		VkClearValue clearValue;
		VkClearValue depthClear;
		clearValue.color = {{ 0.0f, 0.0f, 255, 1.0f }};
		depthClear.depthStencil.depth = 1.f;
		VkClearValue clearValues[] = { clearValue, depthClear };
		VkRenderPassBeginInfo rpInfo = VulkanInitializer::RenderPassBeginInfo(renderpass.Get(), windowExtent,
				frameBuffer[swapchainImageIndex]);
		rpInfo.clearValueCount = 2;
		rpInfo.pClearValues = &clearValues[0];
		frame._mainCommandBuffer->BeginRenderPass(rpInfo);
		{
			drawObjects(allocator, *frame._mainCommandBuffer, frame, sceneParameterBuffer);
		}
		frame._mainCommandBuffer->EndRenderPass();
	}
	frame._mainCommandBuffer->End();

	queue.Submit(frame);
	queue.Present(frame, swapchain, swapchainImageIndex);
	//increase the number of frames drawn
	_frameNumber++;
}