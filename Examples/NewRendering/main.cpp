//
// Created by arthur on 12/07/2022.
//
#include <vulkan/vulkan.h>
#define GLFW_INCLUDE_VULKAN
#include <Concerto/Graphics/Window/GlfW3.hpp>
#include <Concerto/Graphics/Vulkan/Wrapper/Device.hpp>
#include <Concerto/Graphics/Vulkan/Wrapper/RenderPass.hpp>
#include <Concerto/Graphics/Vulkan/Wrapper/ShaderModule.hpp>
#include <Concerto/Graphics/Vulkan/Wrapper/PipelineLayout.hpp>
#include <Concerto/Graphics/Vulkan/Wrapper/Sampler.hpp>
#include <Concerto/Graphics/Vulkan/Wrapper/Swapchain.hpp>
#include <Concerto/Graphics/Vulkan/Wrapper/Pipeline.hpp>
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
#include <Concerto/Core/Logger.hpp>
#include <NZSL/Parser.hpp>
#include <NZSL/SpirvWriter.hpp>
#include <NZSL/Ast/ReflectVisitor.hpp>
#include <Vulkan.hpp>

using namespace Concerto;
using namespace Concerto::Graphics;

int main()
{
	try
	{
		RendererInfo info ={
			.applicationName = "Test",
			.applicationVersion = { 1, 0, 0 },
			.width = 800,
			.height = 600,
		};
		GlfW3 window("Concerto Graphics", 1280, 720);
		Vulkan vulkan(info);
		auto& vkInstance = vulkan.GetVkInstance();
		Device* device = vulkan.CreateDevice(DeviceType::Dedicated);
		auto gpuMinimumAlignment = device->GetPhysicalDevice().GetProperties().limits.minUniformBufferOffsetAlignment;
		VkSurfaceKHR surface = {};
		if (glfwVulkanSupported() == 0)
			Logger::Error("Vulkan not supported");
		auto res = glfwCreateWindowSurface(*vkInstance.Get(), (GLFWwindow*)window.GetRawWindow(), nullptr,&surface);

		device->GetPhysicalDevice().SetSurface(surface);
		Swapchain swapchain(*device, window);
		nzsl::Ast::ModulePtr shaderAst = nzsl::ParseFromFile(".\\Shaders\\tri_mesh_ssbo.nzsl");
		ShaderReflection reflection;
		reflection.Reflect(*shaderAst);
		PipelineLayout meshPipelineLayout = reflection.BuildPipelineLayout(*device);

	
	/*
		const std::size_t sceneParamBufferSize = 2 * PadUniformBuffer(sizeof(GPUSceneData), gpuMinimumAlignment);
		Graphics::Buffer sceneParameterBuffer(device->GetAllocator(), sceneParamBufferSize, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, VMA_MEMORY_USAGE_CPU_TO_GPU);
		std::vector<FrameData> frames;
		frames.reserve(swapchain.GetImages().size());
		for (std::size_t i = 0; i < frames.capacity(); i++)
		{
			frames.emplace_back(*device, 
				device->GetQueueFamilyIndex(Graphics::Queue::Type::Graphics),
				descriptorPool, 
				globalSetLayout, 
				objectSetLayout, 
				sceneParameterBuffer);
		}
	
		ShaderModule colorMeshShader(*device, ".\\Shaders\\default_lit.frag.spv");
		ShaderModule textureMeshShader(*device, ".\\Shaders\\textured_lit.frag.spv");
		ShaderModule meshVertShader(*device, ".\\Shaders\\tri_mesh_ssbo.vert.spv");
	
		PipelineLayout meshPipelineLayout(makePipelineLayout<MeshPushConstants>(*device, { globalSetLayout, objectSetLayout }));
		PipelineLayout texturedSetLayout(makePipelineLayout<MeshPushConstants>(*device, { globalSetLayout, objectSetLayout, singleTextureSetLayout }));
		PipelineInfo meshPipelineInfo({ VulkanInitializer::PipelineShaderStageCreateInfo(VK_SHADER_STAGE_VERTEX_BIT, *meshVertShader.Get()), VulkanInitializer::PipelineShaderStageCreateInfo(VK_SHADER_STAGE_FRAGMENT_BIT, *colorMeshShader.Get()) }, { window.GetWidth(), window.GetHeight()}, meshPipelineLayout);
		Pipeline coloredShaderPipeline(*device, meshPipelineInfo);
		coloredShaderPipeline.BuildPipeline(*swapchain.GetRenderPass()->Get());
	
		meshPipelineInfo._shaderStages.clear();
		meshPipelineInfo._shaderStages.push_back(VulkanInitializer::PipelineShaderStageCreateInfo(VK_SHADER_STAGE_VERTEX_BIT, *meshVertShader.Get()));
		meshPipelineInfo._shaderStages.push_back(VulkanInitializer::PipelineShaderStageCreateInfo(VK_SHADER_STAGE_FRAGMENT_BIT,*textureMeshShader.Get()));
		meshPipelineInfo._pipelineLayout = *texturedSetLayout.Get();
		Pipeline texturedPipeline(*device, meshPipelineInfo);
		texturedPipeline.BuildPipeline(*swapchain.GetRenderPass()->Get());
	
		DescriptorAllocator descriptorAllocator(*device);
		DescriptorLayoutCache descriptorLayoutCache(*device);
		Sampler sampler(*device, VK_FILTER_NEAREST);
		MaterialBuilder materialBuilder(descriptorLayoutCache, descriptorAllocator, sampler);

		Queue& graphicsQueue = device->GetQueue(Graphics::Queue::Type::Graphics);
		UploadContext uploadContext(*device, graphicsQueue.GetFamilyIndex());

		Mesh mesh(Primitive::MakeCube(5));
	
		auto drawFunc = []() {};*/
	
		/*while (!window.ShouldClose())
		{
		
		}*/
	}
	catch (const std::exception& e)
	{
		std::cout << e.what() << std::endl;
	}
	return 0;
}
