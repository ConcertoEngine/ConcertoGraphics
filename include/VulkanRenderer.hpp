//
// Created by arthur on 25/10/2022.
//

#ifndef CONCERTOGRAPHICS_VULKANRENDERER_HPP
#define CONCERTOGRAPHICS_VULKANRENDERER_HPP

#include <string>
#include <optional>
#include <unordered_map>
#include <functional>
#include <utility>
#include "window/GlfW3.hpp"
#include "Version.hpp"
#include "Vulkan/Wrapper/Instance.hpp"
#include "Vulkan/Wrapper/Device.hpp"
#include "Vulkan/Wrapper/PhysicalDevice.hpp"
#include "Vulkan/Wrapper/Allocator.hpp"
#include "Vulkan/Wrapper/Allocator.hpp"
#include "Vulkan/Wrapper/Swapchain.hpp"
#include "Vulkan/Wrapper/RenderPass.hpp"
#include "Vulkan/Wrapper/FrameBuffer.hpp"
#include "Vulkan/Wrapper/DescriptorSetLayout.hpp"
#include "Vulkan/Wrapper/DescriptorPool.hpp"
#include "Vulkan/Wrapper/Buffer.hpp"
#include "Frame.hpp"
#include "Vulkan/Wrapper/ShaderModule.hpp"
#include "Vulkan/Wrapper/PipelineLayout.hpp"
#include "Vulkan/Wrapper/Pipeline.hpp"
#include "Vulkan/Wrapper/Queue.hpp"
#include "UploadContext.hpp"
#include "Vulkan/RenderObject.hpp"
#include "Vulkan/Texture.hpp"
#include "ImGUI.hpp"
#include "TextureBuilder.hpp"
#include "MaterialBuilder.hpp"
#include "Vulkan/Wrapper/Sampler.hpp"
#include "Vulkan/DescriptorLayoutCache.hpp"
#include "Vulkan/DescriptorAllocator.hpp"
#include "Mesh.hpp"
#include "Vulkan/VkMesh.hpp"

namespace Concerto::Graphics
{
	struct RendererInfo
	{
		std::string applicationName;
		Version applicationVersion;
		bool useImGUI = false;
		std::uint32_t width;
		std::uint32_t height;
	};

	class VulkanRenderer
	{
	 public:
		explicit VulkanRenderer(RendererInfo info, GlfW3& window);

		~VulkanRenderer() = default;

		VulkanRenderer(const VulkanRenderer&) = delete;

		VulkanRenderer(VulkanRenderer&&) = delete;

		VulkanRenderer& operator=(const VulkanRenderer&) = delete;

		VulkanRenderer& operator=(VulkanRenderer&&) = delete;

		VulkanRenderer* Instance();

		void Draw(const Camera& camera);
		void DrawObject(MeshPtr &mesh,
			const std::string& texturePath,
			float px,
			float py,
			float pz,
			float rx,
			float ry,
			float rz,
			float sx,
			float sy,
			float sz);
		void UseImGUI();
		ImGUI* GetImGUIContext();
		void UpdateSceneParameters(const Scene& sceneData);
		void Resize(std::uint32_t width, std::uint32_t height);
	 private:
		VkMeshPtr LoadModelIfNotExist(MeshPtr &mesh, const std::string& texturePath);
		void DrawObjects(const Camera& camera);
		std::vector<const char*> _deviceExtensions = { VK_KHR_SWAPCHAIN_EXTENSION_NAME };
		GlfW3& _window;
		RendererInfo _renderInfo;
		Scene _sceneParameters;
		VulkanRenderer* _instance{ nullptr };
		std::uint32_t _frameNumber{ 0 };
		Wrapper::Instance _vulkanInstance;
		VkSurfaceKHR _surface{ VK_NULL_HANDLE };
		Wrapper::PhysicalDevice _physicalDevice;
		Wrapper::Device _device;
		bool _isResized{ false };
		VkPhysicalDeviceProperties _gpuProperties;
		std::uint32_t _graphicsQueueFamilyIndex;
		std::unique_ptr<Graphics::ImGUI> _imGUI;
		std::unique_ptr<Wrapper::Allocator> _allocator;
		std::unique_ptr<Wrapper::Swapchain> _swapchain;
		std::unique_ptr<Wrapper::RenderPass> _renderPass;
		std::vector<Wrapper::FrameBuffer> _frameBuffers;
		std::unique_ptr<Wrapper::DescriptorSetLayout> _globalSetLayout;
		std::unique_ptr<Wrapper::DescriptorSetLayout> _objectSetLayout;
		std::unique_ptr<Wrapper::DescriptorSetLayout> _singleTextureSetLayout;
		std::unique_ptr<Wrapper::DescriptorPool> _descriptorPool;
		std::vector<FrameData> _frames;
		std::unique_ptr<Wrapper::Buffer> _sceneParameterBuffer;
		//TODO : Convert all shaders modules into an unordered_map
		std::unique_ptr<Wrapper::ShaderModule> _colorMeshShader;
		std::unique_ptr<Wrapper::ShaderModule> _textureMeshShader;
		std::unique_ptr<Wrapper::ShaderModule> _meshVertShader;
		std::unique_ptr<Wrapper::PipelineLayout> _meshPipelineLayout;
		std::unique_ptr<Wrapper::PipelineLayout> _texturedSetLayout;
		std::unique_ptr<Wrapper::Pipeline> _coloredShaderPipeline;
		std::unique_ptr<Wrapper::Pipeline> _texturedPipeline;
		std::unique_ptr<Wrapper::Queue> _graphicsQueue;
		std::unique_ptr<UploadContext> _uploadContext;
		std::unordered_map<std::string, RenderObjectPtr> _renderObjects;
		std::unordered_map<std::string, VkMeshPtr> _meshes;
		std::unordered_map<std::string, std::shared_ptr<Texture>> _textures;
		std::unordered_map<VkMeshPtr, std::vector<glm::mat4>> _renderObjectsToDraw;
		std::unique_ptr<TextureBuilder> _textureBuilder;
		std::unique_ptr<DescriptorLayoutCache> _descriptorLayoutCache;
		std::unique_ptr<DescriptorAllocator> _descriptorAllocator;
		std::unique_ptr<MaterialBuilder> _materialBuilder;
		std::unique_ptr<Wrapper::Sampler> _sampler;
	};

} // Concerto::Graphics

#endif //CONCERTOGRAPHICS_VULKANRENDERER_HPP
