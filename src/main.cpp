//
// Created by arthur on 12/07/2022.
//
#define VMA_IMPLEMENTATION
#define VMA_VULKAN_VERSION 1000000
#define VKB_DEBUG

#include "MeshPushConstants.hpp"
#include "glm/vec3.hpp"
#include "glm/gtx/transform.hpp"
#include "Vertex.hpp"
#include "wrapper/Swapchain.hpp"
#include "wrapper/RenderPass.hpp"
#include "wrapper/FrameBuffer.hpp"
#include "wrapper/CommandBuffer.hpp"
#include "wrapper/Fence.hpp"
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
#include "Mesh.hpp"
#include "wrapper/Queue.hpp"
#include "Frame.hpp"
#include "Material.hpp"
#include "RenderObject.hpp"
#include "GPUData.hpp"
#include "Utils.hpp"
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

using Frames = std::array<FrameData, 2>;
std::vector<std::unique_ptr<RenderObject>> _renderables;

void drawObjects(CommandBuffer& commandBuffer, AllocatedBuffer& sceneParameterBuffer);

void
draw(Allocator& allocator, Swapchain& swapchain, RenderPass& renderpass, FrameBuffer& frameBuffer, FrameData& frame,
		AllocatedBuffer& sceneParameterBuffer, Queue& queue);


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
	// Pilpline
	ShaderModule fragShader("shaders/default_lit.frag.spv", _device);
	ShaderModule triangleVertexShader(R"(.\shaders\tri_mesh.vert.spv)", _device);
	PipelineLayout meshPipelineLayout = makePipelineLayout<MeshPushConstants>(_device,
			{ globalSetLayout, objectSetLayout });

	PipelineInfo pipelineInfo;

	pipelineInfo._shaderStages.push_back(
			VulkanInitializer::PipelineShaderStageCreateInfo(VK_SHADER_STAGE_VERTEX_BIT,
					triangleVertexShader.getShaderModule()));
	pipelineInfo._shaderStages.push_back(
			VulkanInitializer::PipelineShaderStageCreateInfo(VK_SHADER_STAGE_FRAGMENT_BIT,
					fragShader.getShaderModule()));
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
	CommandPool uploadCommandPool(_device, _graphicsQueueFamily);
	UploadContext uploadContext(_device, _graphicsQueueFamily, uploadCommandPool.get());
	Fence _renderFence(_device);
	std::unique_ptr<Mesh> monkeyMesh = std::make_unique<Mesh>(".\\assets\\monkey_flat.obj", _allocator,
			VK_BUFFER_USAGE_VERTEX_BUFFER_BIT,
			VMA_MEMORY_USAGE_CPU_TO_GPU);
	_renderables.emplace_back(
			std::make_unique<RenderObject>(std::move(monkeyMesh), meshPipelineLayout.get(), _meshPipeline.get(),
					glm::mat4{ 1.0f }));

	Queue queue(vkbDevice);
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

	float framed = (_frameNumber / 120.f);

	_sceneParameters.ambientColor = { sin(framed), 0, cos(framed), 1 };

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
			commandBuffer.bindPipeline(VK_PIPELINE_BIND_POINT_GRAPHICS, object.material._pipeline);
			lastMaterial = &object.material;
			commandBuffer.bindDescriptorSets(VK_PIPELINE_BIND_POINT_GRAPHICS, object.material._pipelineLayout, 0, 1,
					frame.globalDescriptor, uniform_offset);
			commandBuffer.bindDescriptorSets(VK_PIPELINE_BIND_POINT_GRAPHICS, object.material._pipelineLayout, 1, 1,
					frame.objectDescriptor);
		}
		glm::mat4 mesh_matrix = object.transformMatrix;
//		MeshPushConstants constants{};
//		constants.render_matrix = glm::mat4{ 1.0f };
//		commandBuffer.updatePushConstants(object.material._pipelineLayout, constants);
		if (object.mesh.get() != lastMesh)
		{
			commandBuffer.bindVertexBuffers(object.mesh->_vertexBuffer);
			lastMesh = object.mesh.get();
		}
		commandBuffer.draw(object.mesh->_vertices.size(), 1, 0, i);
	}
}

void
draw(Allocator& allocator, Swapchain& swapchain, RenderPass& renderpass, FrameBuffer& frameBuffer, FrameData& frame,
		AllocatedBuffer& sceneParameterBuffer, Queue& queue)
{
	frame._renderFence.wait(1000000000);
	frame._renderFence.reset();
	std::uint32_t swapchainImageIndex = swapchain.acquireNextImage(frame._presentSemaphore, frame._renderFence,
			1000000000);
	frame._mainCommandBuffer.reset();
	frame._mainCommandBuffer.begin();
	{
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
		{
			drawObjects(allocator, frame._mainCommandBuffer, frame, sceneParameterBuffer);
		}
		frame._mainCommandBuffer.endRenderPass();
	}
	frame._mainCommandBuffer.end();

	queue.Submit(frame);
	queue.Present(frame, swapchain, swapchainImageIndex);
	//increase the number of frames drawn
	_frameNumber++;
}