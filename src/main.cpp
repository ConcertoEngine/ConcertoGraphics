//
// Created by arthur on 12/07/2022.
//
#define VMA_IMPLEMENTATION
#define VMA_VULKAN_VERSION 1000000
#define VKB_DEBUG

#include "MeshPushConstants.hpp"
#include "glm/vec3.hpp"
#include "glm/gtx/transform.hpp"
#include "wrapper/Vertex.hpp"
#include "wrapper/Swapchain.hpp"
#include "wrapper/RenderPass.hpp"
#include "wrapper/FrameBuffer.hpp"
#include "wrapper/CommandBuffer.hpp"
#include "wrapper/CommandPool.hpp"
#include "wrapper/Fence.hpp"
#include "wrapper/DescriptorSet.hpp"
#include "wrapper/DescriptorSetLayout.hpp"
#include "wrapper/DescriptorPool.hpp"
#include "wrapper/AllocatedBuffer.hpp"
#include "wrapper/Semaphore.hpp"
#include "window/GlfW3.hpp"
#include "VkBootstrap.h"
#include "vulkan/vulkan.h"
#include "wrapper/VulkanInitializer.hpp"
#include "wrapper/Allocator.hpp"
#include "wrapper/ShaderModule.hpp"
#include "wrapper/Pipeline.hpp"
#include "wrapper/PipelineInfo.hpp"
#include "wrapper/PipelineLayout.hpp"
#include "wrapper/Mesh.hpp"
#include <iostream>
#include <unordered_map>
#include <array>

VkInstance _instance{ VK_NULL_HANDLE };
VkDebugUtilsMessengerEXT _debug_messenger;
VkPhysicalDevice _chosenGPU{ VK_NULL_HANDLE };
VkDevice _device{ VK_NULL_HANDLE };
VkSurfaceKHR _surface{ VK_NULL_HANDLE };
int _frameNumber = 0;
VkExtent2D windowExtent = { 1280, 720 };
VkPhysicalDeviceProperties _gpuProperties{};
using namespace Concerto;
using namespace Concerto::Graphics;
using namespace Concerto::Graphics::Wrapper;
#define MAX_OBJECTS 1000

struct Material
{
	Material(VkPipelineLayout pipelineLayout, VkPipeline pipeline) : _pipelineLayout(pipelineLayout),
																	 _pipeline(pipeline)
	{
	}

	Material() : _pipelineLayout(VK_NULL_HANDLE), _pipeline(VK_NULL_HANDLE)
	{
	}

	bool operator==(const Material& other) const
	{
		return _pipelineLayout == other._pipelineLayout && _pipeline == other._pipeline;
	}

	bool operator!=(const Material& other) const
	{
		return !(*this == other);
	}

	VkPipeline _pipeline;
	VkPipelineLayout _pipelineLayout;
};


struct GPUCameraData
{
	glm::mat4 view;
	glm::mat4 proj;
	glm::mat4 viewproj;
};
struct GPUObjectData
{
	glm::mat4 modelMatrix;
};
struct GPUSceneData
{
	glm::vec4 fogColor; // w is for exponent
	glm::vec4 fogDistances; //x for min, y for max, zw unused.
	glm::vec4 ambientColor;
	glm::vec4 sunlightDirection; //w for sun power
	glm::vec4 sunlightColor;
};

std::size_t pad_uniform_buffer_size(size_t originalSize)
{
	size_t minUboAlignment = _gpuProperties.limits.minUniformBufferOffsetAlignment;
	size_t alignedSize = originalSize;
	if (minUboAlignment > 0)
	{
		alignedSize = (alignedSize + minUboAlignment - 1) & ~(minUboAlignment - 1);
	}
	return alignedSize;
}

struct FrameData
{
	FrameData(Allocator& allocator, VkDevice device, std::uint32_t queueFamily, DescriptorPool& pool,
			DescriptorSetLayout& globalDescriptorSetLayout, DescriptorSetLayout& objectDescriptorSetLayout,
			AllocatedBuffer& sceneParameterBuffer,
			bool signaled = true) : _presentSemaphore(device),
									_commandPool(device, queueFamily),
									_renderSemaphore(device),
									_renderFence(device, signaled),
									_mainCommandBuffer(device, _commandPool.get()),
									_cameraBuffer(makeAllocatedBuffer<GPUCameraData>(allocator,
											VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT,
											VMA_MEMORY_USAGE_CPU_TO_GPU)),
									globalDescriptor(device, pool, globalDescriptorSetLayout),
									_objectBuffer(makeAllocatedBuffer<GPUObjectData>(allocator, MAX_OBJECTS,
											VK_BUFFER_USAGE_STORAGE_BUFFER_BIT,
											VMA_MEMORY_USAGE_CPU_TO_GPU)),
									objectDescriptor(device, pool, objectDescriptorSetLayout)
	{

		VkDescriptorBufferInfo cameraInfo;
		cameraInfo.buffer = _cameraBuffer._buffer;
		cameraInfo.offset = 0;
		cameraInfo.range = sizeof(GPUCameraData);

		VkDescriptorBufferInfo sceneInfo;
		sceneInfo.buffer = sceneParameterBuffer._buffer;
		sceneInfo.offset = 0;
		sceneInfo.range = sizeof(GPUSceneData);

		VkDescriptorBufferInfo objectBufferInfo;
		objectBufferInfo.buffer = _objectBuffer._buffer;
		objectBufferInfo.offset = 0;
		objectBufferInfo.range = sizeof(GPUObjectData) * MAX_OBJECTS;

		VkWriteDescriptorSet cameraWrite = VulkanInitializer::WriteDescriptorBuffer(VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,
				globalDescriptor.get(), &cameraInfo, 0);

		VkWriteDescriptorSet sceneWrite = VulkanInitializer::WriteDescriptorBuffer(
				VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC, globalDescriptor.get(), &sceneInfo, 1);

		VkWriteDescriptorSet objectWrite = VulkanInitializer::WriteDescriptorBuffer(VK_DESCRIPTOR_TYPE_STORAGE_BUFFER,
				objectDescriptor.get(), &objectBufferInfo, 0);

		VkWriteDescriptorSet setWrites[] = { cameraWrite, sceneWrite, objectWrite };

		vkUpdateDescriptorSets(device, 3, setWrites, 0, nullptr);
	}

	FrameData(FrameData&&) = default;

	FrameData() = delete;

	~FrameData() = default;

	Semaphore _presentSemaphore, _renderSemaphore;
	Fence _renderFence;

	CommandPool _commandPool;
	CommandBuffer _mainCommandBuffer;

	AllocatedBuffer _cameraBuffer;
	DescriptorSet globalDescriptor;

	AllocatedBuffer _objectBuffer;
	DescriptorSet objectDescriptor;
};

using Frames = std::array<FrameData, 2>;

struct RenderObject
{
	explicit RenderObject(std::unique_ptr<Mesh> mesh, VkPipelineLayout pipelineLayout, VkPipeline pipeline) : mesh(
			std::move(mesh)), material(pipelineLayout, pipeline), transformMatrix()
	{

	}

	std::unique_ptr<Mesh> mesh;
	Material material;
	glm::mat4 transformMatrix;
};

std::vector<std::unique_ptr<RenderObject>> _renderables;

void drawObjects(CommandBuffer& commandBuffer, AllocatedBuffer& sceneParameterBuffer);

void
draw(Allocator& allocator, Swapchain& swapchain, RenderPass& renderpass, FrameBuffer& frameBuffer,
		VkQueue _graphicsQueue, FrameData& frame, AllocatedBuffer& sceneParameterBuffer);


int main()
{
	const char* appName = "Concerto";
	IWindowPtr window = std::make_unique<GlfW3>(appName, windowExtent.width, windowExtent.height);

	VkSurfaceKHR vkSurface{ VK_NULL_HANDLE };
	vkb::InstanceBuilder instanceBuilder;
	instanceBuilder.use_default_debug_messenger();
	instanceBuilder.set_app_name(appName)
			.request_validation_layers(true)
			.use_default_debug_messenger()
			.require_api_version(1, 1, 0);
	auto system_info_ret = vkb::SystemInfo::get_system_info();
	if (!system_info_ret)
	{
		std::cerr << system_info_ret.error().message() << std::endl;
		return -1;
	}
	auto system_info = system_info_ret.value();
	if (system_info.is_layer_available("VK_LAYER_LUNARG_api_dump"))
	{
//		instanceBuilder.enable_layer("VK_LAYER_LUNARG_api_dump");
	}
	auto instance = instanceBuilder.build();
	_instance = instance.value().instance;
	glfwCreateWindowSurface(_instance, (GLFWwindow*)window->getRawWindow(), nullptr, &vkSurface);
	vkb::PhysicalDeviceSelector selector(instance.value());
	vkb::PhysicalDevice physicalDevice = selector.set_minimum_version(1, 1)
			.set_surface(vkSurface)
			.select()
			.value();
	vkb::DeviceBuilder deviceBuilder(physicalDevice);
	VkPhysicalDeviceShaderDrawParametersFeatures shader_draw_parameters_features = {};
	shader_draw_parameters_features.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_SHADER_DRAW_PARAMETERS_FEATURES;
	shader_draw_parameters_features.pNext = nullptr;
	shader_draw_parameters_features.shaderDrawParameters = VK_TRUE;
	vkb::Device vkbDevice = deviceBuilder.add_pNext(&shader_draw_parameters_features).build().value();
	_device = vkbDevice.device;
	VkPhysicalDevice _physicalDevice = physicalDevice.physical_device;
	VkQueue _graphicsQueue = vkbDevice.get_queue(vkb::QueueType::graphics).value();
	uint32_t _graphicsQueueFamily = vkbDevice.get_queue_index(vkb::QueueType::graphics).value();

	vkGetPhysicalDeviceProperties(_physicalDevice, &_gpuProperties);
	std::cout << "The GPU has  a minimum buffer alignment of : "
			  << _gpuProperties.limits.minUniformBufferOffsetAlignment << std::endl;
	Allocator _allocator(_physicalDevice, _device, _instance);
	Swapchain swapchain(_allocator, windowExtent, _physicalDevice, _device, vkSurface, _instance);

	// Renderpass
	VkAttachmentDescription color_attachment = {};
	color_attachment.format = swapchain.getImageFormat();
	color_attachment.samples = VK_SAMPLE_COUNT_1_BIT;
	color_attachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
	color_attachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
	color_attachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
	color_attachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
	color_attachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
	color_attachment.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

	VkAttachmentReference color_attachment_ref = {};
	color_attachment_ref.attachment = 0;
	color_attachment_ref.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

	VkAttachmentDescription depth_attachment = {};
	// Depth attachment
	depth_attachment.flags = 0;
	depth_attachment.format = swapchain.getDepthFormat();
	depth_attachment.samples = VK_SAMPLE_COUNT_1_BIT;
	depth_attachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
	depth_attachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
	depth_attachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
	depth_attachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
	depth_attachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
	depth_attachment.finalLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

	VkAttachmentReference depth_attachment_ref = {};
	depth_attachment_ref.attachment = 1;
	depth_attachment_ref.layout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

	//we are going to create 1 subpass, which is the minimum you can do
	VkSubpassDescription subpass = {};
	subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
	subpass.colorAttachmentCount = 1;
	subpass.pColorAttachments = &color_attachment_ref;
	//hook the depth attachment into the subpass
	subpass.pDepthStencilAttachment = &depth_attachment_ref;

	//1 dependency, which is from "outside" into the subpass. And we can read or write color
	VkSubpassDependency dependency = {};
	dependency.srcSubpass = VK_SUBPASS_EXTERNAL;
	dependency.dstSubpass = 0;
	dependency.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
	dependency.srcAccessMask = 0;
	dependency.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
	dependency.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;

	//dependency from outside to the subpass, making this subpass dependent on the previous renderpasses
	VkSubpassDependency depth_dependency = {};
	depth_dependency.srcSubpass = VK_SUBPASS_EXTERNAL;
	depth_dependency.dstSubpass = 0;
	depth_dependency.srcStageMask =
			VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT | VK_PIPELINE_STAGE_LATE_FRAGMENT_TESTS_BIT;
	depth_dependency.srcAccessMask = 0;
	depth_dependency.dstStageMask =
			VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT | VK_PIPELINE_STAGE_LATE_FRAGMENT_TESTS_BIT;
	depth_dependency.dstAccessMask = VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;

	RenderPass renderPass(_device, { color_attachment, depth_attachment }, { subpass },
			{ dependency, depth_dependency });
	// Renderpass
	FrameBuffer frameBuffer(_device, swapchain, renderPass);
	// Commands
	VkDescriptorSetLayoutBinding camBufferBind = VulkanInitializer::DescriptorSetLayoutBinding(
			VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, VK_SHADER_STAGE_VERTEX_BIT, 0);
	VkDescriptorSetLayoutBinding sceneBind = VulkanInitializer::DescriptorSetLayoutBinding(
			VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC, VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT, 1);
	VkDescriptorSetLayoutBinding objectBind = VulkanInitializer::DescriptorSetLayoutBinding(
			VK_DESCRIPTOR_TYPE_STORAGE_BUFFER, VK_SHADER_STAGE_VERTEX_BIT, 0);
	DescriptorSetLayout globalSetLayout(_device, { camBufferBind, sceneBind });
	DescriptorSetLayout objectSetLayout(_device, { objectBind });

	std::vector<VkDescriptorPoolSize> sizes =
			{
					{ VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,         10 },
					{ VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC, 10 },
					{ VK_DESCRIPTOR_TYPE_STORAGE_BUFFER,         10 }
			};
	DescriptorPool descriptorPool(_device, sizes);
	const std::size_t sceneParamBufferSize = 2 * pad_uniform_buffer_size(sizeof(GPUSceneData));
	AllocatedBuffer _sceneParameterBuffer(_allocator, sceneParamBufferSize, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT,
			VMA_MEMORY_USAGE_CPU_TO_GPU);
	Frames frames = {
			FrameData(_allocator, _device, _graphicsQueueFamily, descriptorPool, globalSetLayout, objectSetLayout,
					_sceneParameterBuffer, true),
			FrameData(_allocator, _device, _graphicsQueueFamily, descriptorPool, globalSetLayout, objectSetLayout,
					_sceneParameterBuffer, true)
	};
	// Commands
	// Pilpline
	ShaderModule triangleVertexShader(R"(.\shaders\tri_mesh.vert.spv)", _device);
	std::cout.flush();
	PipelineLayout meshPipelineLayout = makePipelineLayout<MeshPushConstants>(_device,
			{ globalSetLayout, objectSetLayout });

	PipelineInfo pipelineInfo;

	pipelineInfo._shaderStages.push_back(
			VulkanInitializer::PipelineShaderStageCreateInfo(VK_SHADER_STAGE_VERTEX_BIT,
					triangleVertexShader.getShaderModule()));
	std::cout.flush();
	VertexInputDescription vertexDescription = Vertex::getVertexDescription();
	pipelineInfo._vertexInputInfo = VulkanInitializer::VertexInputStateCreateInfo();
	pipelineInfo._vertexInputInfo.pVertexAttributeDescriptions = vertexDescription.attributes.data();
	pipelineInfo._vertexInputInfo.vertexAttributeDescriptionCount = vertexDescription.attributes.size();
	pipelineInfo._vertexInputInfo.pVertexBindingDescriptions = vertexDescription.bindings.data();
	pipelineInfo._vertexInputInfo.vertexBindingDescriptionCount = vertexDescription.bindings.size();
	pipelineInfo._inputAssembly = VulkanInitializer::InputAssemblyCreateInfo(VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST);
	pipelineInfo._viewport.x = 0.0f;
	pipelineInfo._viewport.y = 0.0f;
	pipelineInfo._viewport.width = (float)windowExtent.width;
	pipelineInfo._viewport.height = (float)windowExtent.height;
	pipelineInfo._viewport.minDepth = 0.0f;
	pipelineInfo._viewport.maxDepth = 1.0f;
	pipelineInfo._scissor.offset = { 0, 0 };
	pipelineInfo._scissor.extent = windowExtent;
	pipelineInfo._rasterizer = VulkanInitializer::RasterizationStateCreateInfo(VK_POLYGON_MODE_FILL);
	pipelineInfo._multisampling = VulkanInitializer::MultisamplingStateCreateInfo();
	pipelineInfo._colorBlendAttachment = VulkanInitializer::ColorBlendAttachmentState();
	pipelineInfo._pipelineLayout = meshPipelineLayout.get();
	pipelineInfo._depthStencil = VulkanInitializer::DepthStencilCreateInfo(true, true, VK_COMPARE_OP_LESS_OR_EQUAL);
	std::cout.flush();
	Pipeline _meshPipeline(_device, pipelineInfo);
	_meshPipeline.buildPipeline(renderPass.get()); //TODO RAII
	// Render loop
	std::cout.flush();
	Semaphore _presentSemaphore(_device);
	Semaphore _renderSemaphore(_device);
	Fence _renderFence(_device);
	std::unique_ptr<Mesh> monkeyMesh = std::make_unique<Mesh>(".\\assets\\monkey_flat.obj", _allocator,
			VK_BUFFER_USAGE_VERTEX_BUFFER_BIT,
			VMA_MEMORY_USAGE_CPU_TO_GPU);
	_renderables.emplace_back(
			std::make_unique<RenderObject>(std::move(monkeyMesh), meshPipelineLayout.get(), _meshPipeline.get()));

	std::cout.flush();
	while (true)
	{
		window->popEvent();
		draw(_allocator, swapchain, renderPass, frameBuffer, _graphicsQueue, frames[_frameNumber % frames.size()],
				_sceneParameterBuffer);
	}
	// Render loop
}

void
drawObjects(Allocator& allocator, CommandBuffer& commandBuffer, FrameData& frame, AllocatedBuffer& sceneParameterBuffer)
{
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

	void* data;
	vmaMapMemory(allocator._allocator, frame._cameraBuffer._allocation, &data);

	std::memcpy(data, &camData, sizeof(GPUCameraData));

	vmaUnmapMemory(allocator._allocator, frame._cameraBuffer._allocation);

	float framed = (_frameNumber / 120.f);

	_sceneParameters.ambientColor = { sin(framed), 0, cos(framed), 1 };

	char* sceneData;
	vmaMapMemory(allocator._allocator, sceneParameterBuffer._allocation, (void**)&sceneData);

	int frameIndex = _frameNumber % 2;

	sceneData += pad_uniform_buffer_size(sizeof(GPUSceneData)) * frameIndex;

	std::memcpy(sceneData, &_sceneParameters, sizeof(GPUSceneData));

	vmaUnmapMemory(allocator._allocator, sceneParameterBuffer._allocation);


	void* objectData;
	vmaMapMemory(allocator._allocator, frame._objectBuffer._allocation, &objectData);

	auto* objectSSBO = (GPUObjectData*)objectData;
	for (std::size_t i = 0; i < _renderables.size(); i++)
	{
		objectSSBO[i].modelMatrix = glm::mat4{ 1.0f };
	}
	vmaUnmapMemory(allocator._allocator, frame._objectBuffer._allocation);
	for (std::size_t i = 0; i < _renderables.size(); i++)
	{
		RenderObject& object = *_renderables[i];

		if (lastMaterial == nullptr || object.material != *lastMaterial)
		{
			std::uint32_t uniform_offset = pad_uniform_buffer_size(sizeof(GPUSceneData)) * frameIndex;
			commandBuffer.bindPipeline(VK_PIPELINE_BIND_POINT_GRAPHICS, object.material._pipeline);
			lastMaterial = &object.material;
			commandBuffer.bindDescriptorSets(VK_PIPELINE_BIND_POINT_GRAPHICS, object.material._pipelineLayout, 0, 1,
					frame.globalDescriptor, uniform_offset);
			commandBuffer.bindDescriptorSets(VK_PIPELINE_BIND_POINT_GRAPHICS, object.material._pipelineLayout, 1, 1,
					frame.objectDescriptor);
		}
		glm::mat4 mesh_matrix = object.transformMatrix;
		MeshPushConstants constants{};
		constants.render_matrix = glm::mat4{ 1.0f };
		commandBuffer.updatePushConstants(object.material._pipelineLayout, constants);
		if (object.mesh.get() != lastMesh)
		{
			commandBuffer.bindVertexBuffers(object.mesh->_vertexBuffer);
			lastMesh = object.mesh.get();
		}
		commandBuffer.draw(object.mesh->_vertices.size(), 1, 0, i);
	}
}

void
draw(Allocator& allocator, Swapchain& swapchain, RenderPass& renderpass, FrameBuffer& frameBuffer,
		VkQueue _graphicsQueue, FrameData& frame, AllocatedBuffer& sceneParameterBuffer)
{
	frame._renderFence.wait(1000000000);
	frame._renderFence.reset();
	std::uint32_t swapchainImageIndex = swapchain.acquireNextImage(frame._presentSemaphore, frame._renderFence,
			1000000000);
	frame._mainCommandBuffer.reset();
	frame._mainCommandBuffer.begin();
	VkClearValue clearValue;
	VkClearValue depthClear;
	float flash = std::abs(std::sin(_frameNumber / 120.f));
	clearValue.color = {{ 0.0f, 0.0f, flash, 1.0f }};
	depthClear.depthStencil.depth = 1.f;
	VkClearValue clearValues[] = { clearValue, depthClear };
	VkRenderPassBeginInfo rpInfo = VulkanInitializer::RenderPassBeginInfo(renderpass.get(), windowExtent,
			frameBuffer[swapchainImageIndex]);
	rpInfo.clearValueCount = 2;
	rpInfo.pClearValues = &clearValues[0];
	frame._mainCommandBuffer.beginRenderPass(rpInfo);
	drawObjects(allocator, frame._mainCommandBuffer, frame, sceneParameterBuffer);
	frame._mainCommandBuffer.endRenderPass();
	frame._mainCommandBuffer.end();

	VkSubmitInfo submit = {};
	VkPipelineStageFlags waitStage = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
	auto vkPresentSemaphore = frame._presentSemaphore.get();
	auto vkRenderSemaphore = frame._renderSemaphore.get();
	auto vkCommandBuffer = frame._mainCommandBuffer.get();

	submit.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
	submit.pNext = nullptr;
	submit.pWaitDstStageMask = &waitStage;
	submit.waitSemaphoreCount = 1;
	submit.pWaitSemaphores = &vkPresentSemaphore;
	submit.signalSemaphoreCount = 1;
	submit.pSignalSemaphores = &vkRenderSemaphore;
	submit.commandBufferCount = 1;
	submit.pCommandBuffers = &vkCommandBuffer;
	//TODO Create an object Queue
	if (vkQueueSubmit(_graphicsQueue, 1, &submit, frame._renderFence.get()) != VK_SUCCESS)
	{
		throw std::runtime_error("vkQueueSubmit fail");
	}

	VkPresentInfoKHR presentInfo = {};
	auto vkSwapchain = swapchain.get();
	presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
	presentInfo.pNext = nullptr;

	presentInfo.pSwapchains = &vkSwapchain;
	presentInfo.swapchainCount = 1;

	presentInfo.pWaitSemaphores = &vkRenderSemaphore;
	presentInfo.waitSemaphoreCount = 1;

	presentInfo.pImageIndices = &swapchainImageIndex;

	if (vkQueuePresentKHR(_graphicsQueue, &presentInfo) != VK_SUCCESS)
	{
		throw std::runtime_error("vkQueuePresentKHR fail");
	}

	//increase the number of frames drawn
	_frameNumber++;
}