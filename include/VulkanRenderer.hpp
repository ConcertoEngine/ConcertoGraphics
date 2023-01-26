//
// Created by arthur on 25/10/2022.
//

#ifndef CONCERTOGRAPHICS_VULKANRENDERER_HPP
#define CONCERTOGRAPHICS_VULKANRENDERER_HPP

#include <string>
#include <optional>
#include <unordered_map>
#include <functional>
#include "window/GlfW3.hpp"
#include "Version.hpp"
#include "wrapper/Instance.hpp"
#include "wrapper/Device.hpp"
#include "wrapper/PhysicalDevice.hpp"
#include "wrapper/Allocator.hpp"
#include "wrapper/Allocator.hpp"
#include "wrapper/Swapchain.hpp"
#include "wrapper/RenderPass.hpp"
#include "wrapper/FrameBuffer.hpp"
#include "wrapper/DescriptorSetLayout.hpp"
#include "wrapper/DescriptorPool.hpp"
#include "wrapper/Buffer.hpp"
#include "Frame.hpp"
#include "wrapper/ShaderModule.hpp"
#include "wrapper/PipelineLayout.hpp"
#include "wrapper/Pipeline.hpp"
#include "wrapper/Queue.hpp"
#include "UploadContext.hpp"
#include "RenderObject.hpp"
#include "Texture.hpp"
#include "ImGUI.hpp"

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
		void DrawObject(const std::string& modelPath,
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
		RenderObject& LoadModelIfNotExist(const std::string& modelPath,
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
		Texture& CreateTextureIfNotExist(const std::string& texturePath);
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
		std::unordered_map<std::string, std::unique_ptr<RenderObject>> _renderObjects;
		std::unordered_map<std::string, std::shared_ptr<Texture>> _textures;
		std::vector<std::reference_wrapper<RenderObject>> _renderObjectsToDraw;
	};

} // Concerto::Graphics

#endif //CONCERTOGRAPHICS_VULKANRENDERER_HPP
