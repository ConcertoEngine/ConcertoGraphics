//
// Created by arthur on 12/07/2022.
//
#define VMA_IMPLEMENTATION
#define VMA_VULKAN_VERSION 1000000
#define VKB_DEBUG
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
#include <iostream>

VkInstance _instance {VK_NULL_HANDLE};
VkDebugUtilsMessengerEXT _debug_messenger;
VkPhysicalDevice _chosenGPU {VK_NULL_HANDLE};
VkDevice _device {VK_NULL_HANDLE};
VkSurfaceKHR _surface {VK_NULL_HANDLE};
int _frameNumber = 0;
VkExtent2D windowExtent = { 1280, 720 };
using namespace Concerto;
using namespace Concerto::Graphics;
using namespace Concerto::Graphics::Wrapper;
void draw(Fence &_renderFence, Swapchain &swapchain, Semaphore &_presentSemaphore, Semaphore &_renderSemaphore,
		CommandBuffer &commandBuffer, RenderPass &renderpass, FrameBuffer &frameBuffer, VkQueue _graphicsQueue,
		Pipeline &_trianglePipeline);
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
	if (!system_info_ret) {
		std::cerr << system_info_ret.error().message() << std::endl;
		return -1;
	}
	auto system_info = system_info_ret.value();
	if (system_info.validation_layers_available){
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

	CommandPool commandPool(_device , _graphicsQueueFamily);
	CommandBuffer commandBuffer(_device, commandPool.get());

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

	VkSubpassDescription subpass = {};
	subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
	subpass.colorAttachmentCount = 1;
	subpass.pColorAttachments = &color_attachment_ref;

	VkSubpassDependency dependency = {};
	dependency.srcSubpass = VK_SUBPASS_EXTERNAL;
	dependency.dstSubpass = 0;
	dependency.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
	dependency.srcAccessMask = 0;
	dependency.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
	dependency.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;

	RenderPass renderPass(_device, {color_attachment}, {subpass}, {dependency});
	// Renderpass
	FrameBuffer frameBuffer(_device, swapchain, renderPass);

	// Pilpline
	ShaderModule triangleFragShader(R"(.\shaders\colored_triangle.frag.spv)", _device);
	ShaderModule triangleVertexShader(R"(.\shaders\colored_triangle.vert.spv)", _device);
	PipelineLayout _trianglePipelineLayout(_device, 0, {});

	PipelineInfo pipelineInfo;

	pipelineInfo._shaderStages.push_back(
			VulkanInitializer::PipelineShaderStageCreateInfo(VK_SHADER_STAGE_VERTEX_BIT, triangleVertexShader.getShaderModule()));

	pipelineInfo._shaderStages.push_back(
			VulkanInitializer::PipelineShaderStageCreateInfo(VK_SHADER_STAGE_FRAGMENT_BIT, triangleFragShader.getShaderModule()));


	//vertex input controls how to read vertices from vertex buffers. We aren't using it yet
	pipelineInfo._vertexInputInfo = VulkanInitializer::VertexInputStateCreateInfo();

	//input assembly is the configuration for drawing triangle lists, strips, or individual points.
	//we are just going to draw triangle list
	pipelineInfo._inputAssembly = VulkanInitializer::InputAssemblyCreateInfo(VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST);

	//build viewport and scissor from the swapchain extents
	pipelineInfo._viewport.x = 0.0f;
	pipelineInfo._viewport.y = 0.0f;
	pipelineInfo._viewport.width = (float)windowExtent.width;
	pipelineInfo._viewport.height = (float)windowExtent.height;
	pipelineInfo._viewport.minDepth = 0.0f;
	pipelineInfo._viewport.maxDepth = 1.0f;

	pipelineInfo._scissor.offset = { 0, 0 };
	pipelineInfo._scissor.extent = windowExtent;

	//configure the rasterizer to draw filled triangles
	pipelineInfo._rasterizer = VulkanInitializer::RasterizationStateCreateInfo(VK_POLYGON_MODE_FILL);

	//we don't use multisampling, so just run the default one
	pipelineInfo._multisampling = VulkanInitializer::MultisamplingStateCreateInfo();

	//a single blend attachment with no blending and writing to RGBA
	pipelineInfo._colorBlendAttachment = VulkanInitializer::ColorBlendAttachmentState();

	//use the triangle layout we created
	pipelineInfo._pipelineLayout = _trianglePipelineLayout.get();

 	Pipeline _trianglePipeline(_device, pipelineInfo);
	_trianglePipeline.buildPipeline(renderPass.get()); //TODO RAII
	// Render loop
	Semaphore _presentSemaphore(_device);
	Semaphore _renderSemaphore(_device);
	Fence _renderFence(_device);

	while (true)
	{
		draw(_renderFence, swapchain, _presentSemaphore, _renderSemaphore, commandBuffer, renderPass, frameBuffer, _graphicsQueue, _trianglePipeline);
	}
	// Render loop
}

void draw(Fence &_renderFence, Swapchain &swapchain, Semaphore &_presentSemaphore, Semaphore &_renderSemaphore,
		CommandBuffer &commandBuffer, RenderPass &renderpass, FrameBuffer &frameBuffer, VkQueue _graphicsQueue,
		Pipeline &_trianglePipeline)
{
	_renderFence.wait(1000000000);
	_renderFence.reset();
	std::uint32_t swapchainImageIndex = swapchain.acquireNextImage(_presentSemaphore, _renderFence, 1000000000);
	commandBuffer.reset();
	commandBuffer.begin();
	VkClearValue clearValue;
	float flash = std::abs(std::sin(_frameNumber / 120.f));
	clearValue.color = { { 0.0f, 0.0f, flash, 1.0f } };

	VkRenderPassBeginInfo rpInfo = {};
	rpInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
	rpInfo.pNext = nullptr;

	rpInfo.renderPass = renderpass.get();
	rpInfo.renderArea.offset.x = 0;
	rpInfo.renderArea.offset.y = 0;
	rpInfo.renderArea.extent = windowExtent;
	rpInfo.framebuffer = frameBuffer[swapchainImageIndex];

	//connect clear values
	rpInfo.clearValueCount = 1;
	rpInfo.pClearValues = &clearValue;
	commandBuffer.beginRenderPass(rpInfo);
	commandBuffer.bindPipeline(VK_PIPELINE_BIND_POINT_GRAPHICS, _trianglePipeline);
	commandBuffer.draw(3, 1, 0, 0);
	commandBuffer.endRenderPass();
	commandBuffer.end();

	VkSubmitInfo submit = {};
	VkPipelineStageFlags waitStage = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
	auto vkPresentSemaphore =  _presentSemaphore.get();
	auto vkRenderSemaphore = _renderSemaphore.get();
	auto vkCommandBuffer = commandBuffer.get();

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
	if (vkQueueSubmit(_graphicsQueue, 1, &submit, _renderFence.get()) != VK_SUCCESS)
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

	if(vkQueuePresentKHR(_graphicsQueue, &presentInfo) != VK_SUCCESS)
	{
		throw std::runtime_error("vkQueuePresentKHR fail");
	}

	//increase the number of frames drawn
	_frameNumber++;
}