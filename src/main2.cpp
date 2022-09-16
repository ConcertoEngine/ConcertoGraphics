////
//// Created by arthur on 09/06/22.
////
//#define VMA_IMPLEMENTATION
//#define VMA_VULKAN_VERSION 1000000
//#define VKB_DEBUG
//#include "wrapper/Vertex.hpp"
//#include "wrapper/Swapchain.hpp"
//#include "wrapper/RenderPass.hpp"
//#include "wrapper/FrameBuffer.hpp"
//#include "wrapper/CommandBuffer.hpp"
//#include "wrapper/CommandPool.hpp"
//#include "wrapper/Fence.hpp"
//#include "wrapper/DescriptorSet.hpp"
//#include "wrapper/DescriptorSetLayout.hpp"
//#include "wrapper/DescriptorPool.hpp"
//#include "wrapper/AllocatedBuffer.hpp"
//#include "wrapper/Semaphore.hpp"
//#include "window/GlfW3.hpp"
//#include "VkBootstrap.h"
//#include "wrapper/VulkanInitializer.hpp"
//#include "wrapper/Allocator.hpp"
//#include "wrapper/ShaderModule.hpp"
//#include "wrapper/Pipeline.hpp"
//#include "wrapper/PipelineInfo.hpp"
//#include "wrapper/PipelineLayout.hpp"
//#include <iostream>
//#include <unordered_map>
//
//#define FRAME_OVERLAP 2
//using namespace Concerto::Graphics::Wrapper;
//using namespace Concerto;
//
//struct GPUSceneData
//{
//	glm::vec4 fogColor; // w is for exponent
//	glm::vec4 fogDistances; //x for min, y for max, zw unused.
//	glm::vec4 ambientColor;
//	glm::vec4 sunlightDirection; //w for sun power
//	glm::vec4 sunlightColor;
//};
//
//struct GPUCameraData
//{
//	glm::mat4 view;
//	glm::mat4 proj;
//	glm::mat4 viewproj;
//};
//
//struct GPUObjectData
//{
//	glm::mat4 modelMatrix;
//};
//
//struct MeshPushConstants
//{
//	glm::vec4 data;
//	glm::mat4 render_matrix;
//};
//
//struct FrameData
//{
//	FrameData(Allocator& allocator, VkDevice device, std::uint32_t queueFamily, DescriptorPool& pool,
//			DescriptorSetLayout& globalDescriptorSetLayout, DescriptorSetLayout& objectDescriptorSetLayout,
//			AllocatedBuffer& sceneParameterBuffer,
//			bool signaled = true) : _presentSemaphore(device),
//									_commandPool(device, queueFamily),
//									_renderSemaphore(device),
//									_renderFence(device, signaled),
//									_mainCommandBuffer(device, _commandPool.Get()),
//									_cameraBuffer(MakeAllocatedBuffer<GPUCameraData>(allocator,
//											VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT,
//											VMA_MEMORY_USAGE_CPU_TO_GPU)),
//									globalDescriptor(device, pool, globalDescriptorSetLayout),
//									_objectBuffer(MakeAllocatedBuffer<GPUObjectData>(allocator, 1000,
//											VK_BUFFER_USAGE_STORAGE_BUFFER_BIT,
//											VMA_MEMORY_USAGE_CPU_TO_GPU)),
//									objectDescriptor(device, pool, objectDescriptorSetLayout)
//	{
//
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
//				globalDescriptor.Get(), &cameraInfo, 0);
//
//		VkWriteDescriptorSet sceneWrite = VulkanInitializer::WriteDescriptorBuffer(
//				VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC, globalDescriptor.Get(), &sceneInfo, 1);
//
//		VkWriteDescriptorSet objectWrite = VulkanInitializer::WriteDescriptorBuffer(VK_DESCRIPTOR_TYPE_STORAGE_BUFFER,
//				objectDescriptor.Get(), &objectBufferInfo, 0);
//
//		VkWriteDescriptorSet setWrites[] = { cameraWrite, sceneWrite, objectWrite };
//
//		vkUpdateDescriptorSets(device, 3, setWrites, 0, nullptr);
//	}
//
//	FrameData(FrameData&&) = default;
//
//	FrameData() = delete;
//
//	~FrameData() = default;
//
//	Semaphore _presentSemaphore, _renderSemaphore;
//	Fence _renderFence;
//
//	CommandPool _commandPool;
//	CommandBuffer _mainCommandBuffer;
//
//	AllocatedBuffer _cameraBuffer;
//	DescriptorSet globalDescriptor;
//
//	AllocatedBuffer _objectBuffer;
//	DescriptorSet objectDescriptor;
//};
//
//struct UploadContext
//{
//	UploadContext(VkDevice device, std::uint32_t queueFamily, bool signaled = true) : _uploadFence(device, signaled),
//																					  _commandBuffer(device,
//																							  _commandPool.Get()),
//																					  _commandPool(device, queueFamily)
//	{
//	}
//
//	~UploadContext() = default;
//
//	Fence _uploadFence;
//	CommandPool _commandPool;
//	CommandBuffer _commandBuffer;
//};
//
//std::size_t padUniformBufferSize(size_t originalSize, VkPhysicalDeviceProperties gpuProperties)
//{
//	size_t minUboAlignment = gpuProperties.limits.minUniformBufferOffsetAlignment;
//	size_t alignedSize = originalSize;
//	if (minUboAlignment > 0)
//	{
//		alignedSize = (alignedSize + minUboAlignment - 1) & ~(minUboAlignment - 1);
//	}
//	return alignedSize;
//}
//
//int main()
//{
//	const char* appName = "Concerto";
//	VkExtent2D windowExtent = { 800, 600 };
//	IWindowPtr window = std::make_unique<GlfW3>(appName, windowExtent.width, windowExtent.height);
//	VkInstance vkInstance{ VK_NULL_HANDLE };
//	VkSurfaceKHR vkSurface{ VK_NULL_HANDLE };
//	vkb::InstanceBuilder _builder;
//	auto instance = _builder.set_app_name(appName)
//			.request_validation_layers(true)
//			.use_default_debug_messenger()
//			.require_api_version(1, 1, 0)
//			.build();
//	auto system_info_ret = vkb::SystemInfo::get_system_info();
//	if (!system_info_ret) {
//		std::cerr << system_info_ret.error().message() << std::endl;
//		return -1;
//	}
//	auto system_info = system_info_ret.value();
//	if (system_info.validation_layers_available){
//		_builder.enable_validation_layers();
//	}
//	vkInstance = instance.value().instance;
//	glfwCreateWindowSurface(vkInstance, (GLFWwindow*)window->getRawWindow(), nullptr, &vkSurface);
//	vkb::PhysicalDeviceSelector selector(instance.value());
//	vkb::PhysicalDevice physicalDevice = selector.set_minimum_version(1, 1)
//			.set_surface(vkSurface)
//			.select()
//			.value();
//	vkb::DeviceBuilder deviceBuilder(physicalDevice);
//	vkb::Device device = deviceBuilder.build().value();
//	VkDevice _device = device.device;
//	VkPhysicalDevice _physicalDevice = physicalDevice.physical_device;
//	VkQueue _graphicsQueue = device.get_queue(vkb::QueueType::graphics).value();
//	uint32_t _graphicsQueueFamily = device.get_queue_index(vkb::QueueType::graphics).value();
//	VkPhysicalDeviceProperties _gpuProperties{};
//	vkGetPhysicalDeviceProperties(_physicalDevice, &_gpuProperties);
//	Allocator _allocator(_physicalDevice, _device, vkInstance);
//
//	Swapchain _swapchain(_allocator, windowExtent, _physicalDevice, _device, vkSurface, vkInstance);
//
//	VkAttachmentDescription color_attachment = {};
//	color_attachment.format = _swapchain.getImageFormat();
//	color_attachment.samples = VK_SAMPLE_COUNT_1_BIT;
//	color_attachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
//	color_attachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
//	color_attachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
//	color_attachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
//	color_attachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
//	color_attachment.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;
//
//	VkAttachmentReference color_attachment_ref = {};
//	color_attachment_ref.attachment = 0;
//	color_attachment_ref.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
//
//	VkAttachmentDescription depth_attachment = {};
//	depth_attachment.flags = 0;
//	depth_attachment.format = _swapchain.getDepthFormat();
//	depth_attachment.samples = VK_SAMPLE_COUNT_1_BIT;
//	depth_attachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
//	depth_attachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
//	depth_attachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
//	depth_attachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
//	depth_attachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
//	depth_attachment.finalLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;
//	VkAttachmentReference depth_attachment_ref = {};
//	depth_attachment_ref.attachment = 1;
//	depth_attachment_ref.layout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;
//
//	//we are going to create 1 subpass, which is the minimum you can do
//	VkSubpassDescription subpass = {};
//	subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
//	subpass.colorAttachmentCount = 1;
//	subpass.pColorAttachments = &color_attachment_ref;
//	//hook the depth attachment into the subpass
//	subpass.pDepthStencilAttachment = &depth_attachment_ref;
//
//	//1 dependency, which is from "outside" into the subpass. And we can read or write color
//	VkSubpassDependency dependency = {};
//	dependency.srcSubpass = VK_SUBPASS_EXTERNAL;
//	dependency.dstSubpass = 0;
//	dependency.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
//	dependency.srcAccessMask = 0;
//	dependency.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
//	dependency.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
//
//	//dependency from outside to the subpass, making this subpass dependent on the previous renderpasses
//	VkSubpassDependency depth_dependency = {};
//	depth_dependency.srcSubpass = VK_SUBPASS_EXTERNAL;
//	depth_dependency.dstSubpass = 0;
//	depth_dependency.srcStageMask =
//			VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT | VK_PIPELINE_STAGE_LATE_FRAGMENT_TESTS_BIT;
//	depth_dependency.srcAccessMask = 0;
//	depth_dependency.dstStageMask =
//			VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT | VK_PIPELINE_STAGE_LATE_FRAGMENT_TESTS_BIT;
//	depth_dependency.dstAccessMask = VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;
//
//	std::vector<VkSubpassDependency> dependencies = { dependency, depth_dependency };
//	std::vector<VkAttachmentDescription> attachments = { color_attachment, depth_attachment };
//	std::vector<VkSubpassDescription> subPasses = { subpass };
//
//	RenderPass _renderPass(_device, attachments, subPasses, dependencies);
//
//	FrameBuffer _frameBuffer(_device, _swapchain, _renderPass);
//	UploadContext _uploadContext(_device, _graphicsQueueFamily);
//	std::vector<VkDescriptorPoolSize> sizes =
//			{
//					{ VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,         10 },
//					{ VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC, 10 },
//					{ VK_DESCRIPTOR_TYPE_STORAGE_BUFFER,         10 },
//					{ VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 10 }
//			};
//	DescriptorPool _descriptorPool(_device, sizes);
//
//	std::vector<VkDescriptorSetLayoutBinding> bindings = {
//			{ VulkanInitializer::DescriptorSetLayoutBinding(
//					VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, VK_SHADER_STAGE_VERTEX_BIT, 0)
//			},
//			{
//			  VulkanInitializer::DescriptorSetLayoutBinding(
//					  VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC,
//					  VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT, 1)
//			}
//	};
//	DescriptorSetLayout _globalSetLayout(_device, bindings);
//
//	std::vector<VkDescriptorSetLayoutBinding> objectBind = {
//			{ VulkanInitializer::DescriptorSetLayoutBinding(
//					VK_DESCRIPTOR_TYPE_STORAGE_BUFFER, VK_SHADER_STAGE_VERTEX_BIT, 0) }
//	};
//	DescriptorSetLayout _objectSetLayout(_device, objectBind);
//	std::vector<VkDescriptorSetLayoutBinding> textureBind = {
//			{ VulkanInitializer::DescriptorSetLayoutBinding(
//					VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, VK_SHADER_STAGE_FRAGMENT_BIT, 0) }
//	};
//	DescriptorSetLayout _singleTextureSetLayout(_device, textureBind);
//
//	const std::size_t sceneParamBufferSize = FRAME_OVERLAP * padUniformBufferSize(sizeof(GPUSceneData), _gpuProperties);
//	AllocatedBuffer sceneParameterBuffer(_allocator, sceneParamBufferSize, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT,
//			VMA_MEMORY_USAGE_CPU_TO_GPU);
//	std::vector<FrameData> _frameData;
//	_frameData.reserve(FRAME_OVERLAP);
//	for (std::uint32_t i = 0; i < FRAME_OVERLAP; i++)
//	{
//		_frameData.emplace_back(_allocator, _device, _graphicsQueueFamily, _descriptorPool, _globalSetLayout,
//				_objectSetLayout, sceneParameterBuffer,
//				VK_FENCE_CREATE_SIGNALED_BIT);
//	}
//	ShaderModule colorMeshShader(R"(C:\Users\arthu\Documents\ConcertoGraphics\shaders\default_lit.frag.spv)", device);
//	ShaderModule _texturedMeshShader(R"(C:\Users\arthu\Documents\ConcertoGraphics\shaders\textured_lit.frag.spv)", device);
//	ShaderModule meshVertShader(R"(C:\Users\arthu\Documents\ConcertoGraphics\shaders\tri_mesh_ssbo.vert.spv)", device);
//	VertexInputDescription vertexDescription = Vertex::getVertexDescription();
//	PipelineInfo pipelineInfo;
//	pipelineInfo._shaderStages = {
//			VulkanInitializer::PipelineShaderStageCreateInfo(VK_SHADER_STAGE_VERTEX_BIT, meshVertShader.getShaderModule()),
//			VulkanInitializer::PipelineShaderStageCreateInfo(VK_SHADER_STAGE_FRAGMENT_BIT, colorMeshShader.getShaderModule())
//	};
//	PipelineLayout meshPipLayout = makePipelineLayout<MeshPushConstants>(_device,
//			{ _globalSetLayout, _objectSetLayout });
////	PipelineLayout texturedPipLayout = makePipelineLayout<MeshPushConstants>(_device,
////			{ _globalSetLayout, _objectSetLayout, _singleTextureSetLayout });
//	pipelineInfo._pipelineLayout = meshPipLayout.Get();
//	pipelineInfo._vertexInputInfo = VulkanInitializer::VertexInputStateCreateInfo();
//	pipelineInfo._inputAssembly = VulkanInitializer::InputAssemblyCreateInfo(VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST);
//	pipelineInfo.viewport = { 0, 0, static_cast<float>(windowExtent.width), static_cast<float>(windowExtent.height),
//							  0.f, 1.f };
//	pipelineInfo._scissor = {{ 0, 0 }, windowExtent };
//	pipelineInfo._rasterizer = VulkanInitializer::RasterizationStateCreateInfo(VK_POLYGON_MODE_FILL);
//	pipelineInfo._multisampling = VulkanInitializer::MultisamplingStateCreateInfo();
//	pipelineInfo._colorBlendAttachment = VulkanInitializer::ColorBlendAttachmentState();
//	pipelineInfo._depthStencil = VulkanInitializer::DepthStencilCreateInfo(true, true, VK_COMPARE_OP_LESS_OR_EQUAL);
//	pipelineInfo._vertexInputInfo.pVertexAttributeDescriptions = vertexDescription.attributes.data();
//	pipelineInfo._vertexInputInfo.vertexAttributeDescriptionCount = vertexDescription.attributes.size();
//	pipelineInfo._vertexInputInfo.pVertexBindingDescriptions = vertexDescription.bindings.data();
//	pipelineInfo._vertexInputInfo.vertexBindingDescriptionCount = vertexDescription.bindings.size();
//
//	Pipeline meshPipeline(_device, pipelineInfo);
//	meshPipeline.createMaterial("defaultmesh", meshPipLayout.Get());
////	pipelineInfo._shaderStages = {
////			VulkanInitializer::PipelineShaderStageCreateInfo(VK_SHADER_STAGE_VERTEX_BIT, meshVertShader.getShaderModule()),
////			VulkanInitializer::PipelineShaderStageCreateInfo(VK_SHADER_STAGE_FRAGMENT_BIT, _texturedMeshShader.getShaderModule())
////	};
////	pipelineInfo._pipelineLayout = texturedPipLayout.Get();
////	Pipeline texPipeline(_device, pipelineInfo);
////	texPipeline.createMaterial("texturedmesh", std::move(texturedPipLayout));
//	while(true) {
//
//	}
//}