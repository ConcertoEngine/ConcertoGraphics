//
// Created by arthur on 12/07/2022.
//
#define VMA_IMPLEMENTATION
#define VMA_VULKAN_VERSION 1000000
#define VKB_DEBUG

#include "MeshPushConstants.hpp"
#include "glm/glm.hpp"
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
#include "wrapper/VulkanInitializer.hpp"
#include "wrapper/Allocator.hpp"
#include "wrapper/ShaderModule.hpp"
#include "wrapper/Pipeline.hpp"
#include "wrapper/PipelineInfo.hpp"
#include "wrapper/PipelineLayout.hpp"
#include "window/GlfW3.hpp"
#include "wrapper/Swapchain.hpp"
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

using namespace Concerto;
using namespace Concerto::Graphics;
using namespace Concerto::Graphics::Wrapper;


struct Material
{
	Material(VkPipelineLayout pipelineLayout, VkPipeline pipeline) : _pipelineLayout(pipelineLayout),
																	 _pipeline(pipeline)
	{
	}

	Material() : _pipelineLayout(VK_NULL_HANDLE)
	{
	}

	VkPipeline _pipeline;
	VkPipelineLayout _pipelineLayout;
};

struct FrameData
{
	FrameData(Allocator& allocator, VkDevice device, std::uint32_t queueFamily, //DescriptorPool& pool,
			//DescriptorSetLayout& globalDescriptorSetLayout, DescriptorSetLayout& objectDescriptorSetLayout,
			//AllocatedBuffer& sceneParameterBuffer,
			bool signaled = true) : _presentSemaphore(device),
									_commandPool(device, queueFamily),
									_renderSemaphore(device),
									_renderFence(device, signaled),
									_mainCommandBuffer(device, _commandPool.get())//,
//									_cameraBuffer(makeAllocatedBuffer<GPUCameraData>(allocator,
//											VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT,
//											VMA_MEMORY_USAGE_CPU_TO_GPU)),
//									globalDescriptor(device, pool, globalDescriptorSetLayout),
//									_objectBuffer(makeAllocatedBuffer<GPUObjectData>(allocator, 1000,
//											VK_BUFFER_USAGE_STORAGE_BUFFER_BIT,
//											VMA_MEMORY_USAGE_CPU_TO_GPU)),
//									objectDescriptor(device, pool, objectDescriptorSetLayout)
	{

//		VkDescriptorBufferInfo cameraInfo;
//		cameraInfo.buffer = _cameraBuffer._buffer;
//		cameraInfo.offset = 0;
//		cameraInfo.range = sizeof(GPUCameraData);
//
//		VkDescriptorBufferInfo sceneInfo;
//		sceneInfo.buffer = sceneParameterBuffer._buffer;
//		sceneInfo.offset = 0;
//		sceneInfo.range = sizeof(GPUSceneData);
//
//		VkDescriptorBufferInfo objectBufferInfo;
//		objectBufferInfo.buffer = _objectBuffer._buffer;
//		objectBufferInfo.offset = 0;
//		objectBufferInfo.range = sizeof(GPUObjectData) * 1000;
//
//		VkWriteDescriptorSet cameraWrite = VulkanInitializer::WriteDescriptorBuffer(VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,
//				globalDescriptor.get(), &cameraInfo, 0);
//
//		VkWriteDescriptorSet sceneWrite = VulkanInitializer::WriteDescriptorBuffer(
//				VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC, globalDescriptor.get(), &sceneInfo, 1);
//
//		VkWriteDescriptorSet objectWrite = VulkanInitializer::WriteDescriptorBuffer(VK_DESCRIPTOR_TYPE_STORAGE_BUFFER,
//				objectDescriptor.get(), &objectBufferInfo, 0);
//
//		VkWriteDescriptorSet setWrites[] = { cameraWrite, sceneWrite, objectWrite };
//
//		vkUpdateDescriptorSets(device, 3, setWrites, 0, nullptr);
	}

	FrameData(FrameData&&) = default;

	FrameData() = delete;

	~FrameData() = default;

	Semaphore _presentSemaphore, _renderSemaphore;
	Fence _renderFence;

	CommandPool _commandPool;
	CommandBuffer _mainCommandBuffer;

//	AllocatedBuffer _cameraBuffer;
//	DescriptorSet globalDescriptor;
//
//	AllocatedBuffer _objectBuffer;
//	DescriptorSet objectDescriptor;
};

using Frames = std::array<FrameData, 2>;

struct RenderObject
{
	explicit RenderObject(std::unique_ptr<Mesh> mesh, VkPipelineLayout pipelineLayout, VkPipeline pipeline) : mesh(
			std::move(mesh)), material(pipelineLayout, pipeline),
																											  transformMatrix()
	{

	}

	std::unique_ptr<Mesh> mesh;
	Material material;
	glm::mat4 transformMatrix;
};

std::vector<std::unique_ptr<RenderObject>> _renderables;

void drawObjects(CommandBuffer& commandBuffer);

void
draw(Swapchain& swapchain, RenderPass& renderpass, FrameBuffer& frameBuffer, VkQueue _graphicsQueue, FrameData& frame);

int main()
{
	const char* appName = "Concerto";
	IWindowPtr window = std::make_unique<GlfW3>(appName, windowExtent.width, windowExtent.height);

	VkSurfaceKHR vkSurface{ VK_NULL_HANDLE };
	vkb::InstanceBuilder _builder;
	auto instance = _builder.set_app_name(appName)
			.request_validation_layers(true)
			.use_default_debug_messenger()
			.require_api_version(1, 1, 0)
			.build();
	auto system_info_ret = vkb::SystemInfo::get_system_info();
	if (!system_info_ret)
	{
		std::cerr << system_info_ret.error().message() << std::endl;
		return -1;
	}
	auto system_info = system_info_ret.value();
	if (system_info.validation_layers_available)
	{
		_builder.enable_validation_layers();
	}
	_instance = instance.value().instance;
	glfwCreateWindowSurface(_instance, (GLFWwindow*)window->getRawWindow(), nullptr, &vkSurface);
	vkb::PhysicalDeviceSelector selector(instance.value());
	vkb::PhysicalDevice physicalDevice = selector.set_minimum_version(1, 1)
			.set_surface(vkSurface)
			.select()
			.value();
	vkb::DeviceBuilder deviceBuilder(physicalDevice);
	vkb::Device device = deviceBuilder.build().value();
	_device = device.device;
	VkPhysicalDevice _physicalDevice = physicalDevice.physical_device;
	VkQueue _graphicsQueue = device.get_queue(vkb::QueueType::graphics).value();
	uint32_t _graphicsQueueFamily = device.get_queue_index(vkb::QueueType::graphics).value();
	VkPhysicalDeviceProperties _gpuProperties{};
	vkGetPhysicalDeviceProperties(_physicalDevice, &_gpuProperties);

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
	Frames frames = {
			FrameData(_allocator, _device, _graphicsQueueFamily, true),
			FrameData(_allocator, _device, _graphicsQueueFamily, true)
	};
	// Commands
	// Pilpline
	ShaderModule triangleFragShader(R"(.\shaders\colored_triangle.frag.spv)", _device);
	ShaderModule triangleVertexShader(R"(.\shaders\tri_mesh.vert.spv)", _device);
	PipelineLayout meshPipelineLayout = makePipelineLayout<MeshPushConstants>(_device, {});

	PipelineInfo pipelineInfo;

	pipelineInfo._shaderStages.push_back(
			VulkanInitializer::PipelineShaderStageCreateInfo(VK_SHADER_STAGE_VERTEX_BIT,
					triangleVertexShader.getShaderModule()));
	pipelineInfo._shaderStages.push_back(
			VulkanInitializer::PipelineShaderStageCreateInfo(VK_SHADER_STAGE_FRAGMENT_BIT,
					triangleFragShader.getShaderModule()));

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

	Pipeline _meshPipeline(_device, pipelineInfo);
	_meshPipeline.buildPipeline(renderPass.get()); //TODO RAII

	// Render loop

	Semaphore _presentSemaphore(_device);
	Semaphore _renderSemaphore(_device);
	Fence _renderFence(_device);
	std::unique_ptr<Mesh> monkeyMesh = std::make_unique<Mesh>(".\\assets\\monkey_flat.obj", _allocator,
			VK_BUFFER_USAGE_VERTEX_BUFFER_BIT,
			VMA_MEMORY_USAGE_CPU_TO_GPU);
	_renderables.emplace_back(
			std::make_unique<RenderObject>(std::move(monkeyMesh), meshPipelineLayout.get(), _meshPipeline.get()));
	while (true)
	{
		window->popEvent();
		draw(swapchain, renderPass, frameBuffer, _graphicsQueue, frames[_frameNumber % frames.size()]);
	}
	// Render loop
}

void drawObjects(CommandBuffer& commandBuffer)
{
	glm::vec3 camPos = { 0.f, 0.f, -2.f };

	glm::mat4 view = glm::translate(glm::mat4(1.f), camPos);
	glm::mat4 projection = glm::perspective(glm::radians(70.f), 1700.f / 900.f, 0.1f, 200.0f);
	projection[1][1] *= -1;

	Mesh* lastMesh = nullptr;
	Material* lastMaterial = nullptr;
	for (auto& object: _renderables)
	{
		if (&object->material != lastMaterial)
		{
			commandBuffer.bindPipeline(VK_PIPELINE_BIND_POINT_GRAPHICS, object->material._pipeline);
			lastMaterial = &object->material;
		}
		glm::mat4 model = glm::mat4{ 1.0f };;
		glm::mat4 mesh_matrix = projection * view * model;

		MeshPushConstants constants{};
		constants.render_matrix = mesh_matrix;
		commandBuffer.updatePushConstants(object->material._pipelineLayout, constants);
		if (object->mesh.get() != lastMesh)
		{
			commandBuffer.bindVertexBuffers(object->mesh->_vertexBuffer);
			lastMesh = object->mesh.get();
		}
		commandBuffer.draw(object->mesh->_vertices.size(), 1, 0, 0);
	}
}

void
draw(Swapchain& swapchain, RenderPass& renderpass, FrameBuffer& frameBuffer, VkQueue _graphicsQueue, FrameData& frame)
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
	drawObjects(frame._mainCommandBuffer);
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