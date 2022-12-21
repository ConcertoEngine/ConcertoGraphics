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
#include "wrapper/AllocatedBuffer.hpp"
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

		void Draw(const Camera &camera);
		void DrawObject(const std::string& modelPath, const std::string& texturePath, float px, float py, float pz, float rx,
				float ry, float rz, float sx, float sy, float sz);
		void UseImGUI();
		ImGUI* GetImGUIContext();
	private:
		RenderObject& LoadModelIfNotExist(const std::string& modelPath, const std::string& texturePath, float px, float py, float pz, float rx,
				float ry, float rz, float sx, float sy, float sz);
		Texture& CreateTextureIfNotExist(const std::string& texturePath);
		void DrawObjects(const Camera &camera);
		std::vector<const char*> _deviceExtensions = { VK_KHR_SWAPCHAIN_EXTENSION_NAME };
		GlfW3& _window;
		RendererInfo _renderInfo;
		VulkanRenderer* _instance{nullptr};
		std::uint32_t _frameNumber{0};
		Wrapper::Instance _vulkanInstance;
		VkSurfaceKHR _surface {VK_NULL_HANDLE};
		Wrapper::PhysicalDevice _physicalDevice;
		Wrapper::Device _device;
		VkPhysicalDeviceProperties _gpuProperties;
		std::uint32_t _graphicsQueueFamilyIndex;
		std::optional<Graphics::ImGUI> _imGUI;
		std::optional<Wrapper::Allocator> _allocator; //TODO : Remove optional
		std::optional<Wrapper::Swapchain> _swapchain; //TODO : Remove optional
		std::optional<Wrapper::RenderPass> _renderPass; //TODO : Remove optional
		std::vector<Wrapper::FrameBuffer> _frameBuffers; //TODO : Remove optional
		std::optional<Wrapper::DescriptorSetLayout> _globalSetLayout; //TODO : Remove optional
		std::optional<Wrapper::DescriptorSetLayout> _objectSetLayout; //TODO : Remove optional
		std::optional<Wrapper::DescriptorSetLayout> _singleTextureSetLayout; //TODO : Remove optional
		std::optional<Wrapper::DescriptorPool> _descriptorPool; //TODO : Remove optional
		std::vector<FrameData> _frames;
		std::optional<Wrapper::AllocatedBuffer> _sceneParameterBuffer; //TODO : Remove optional
		//TODO : Convert all shaders modules into a unordered_map
		std::optional<Wrapper::ShaderModule> _colorMeshShader; //TODO : Remove optional
		std::optional<Wrapper::ShaderModule> _textureMeshShader; //TODO : Remove optional
		std::optional<Wrapper::ShaderModule> _meshVertShader; //TODO : Remove optional
		std::optional<Wrapper::PipelineLayout> _meshPipelineLayout; //TODO : Remove optional
		std::optional<Wrapper::PipelineLayout> _texturedSetLayout; //TODO : Remove optional
		std::optional<Wrapper::Pipeline> _coloredShaderPipeline; //TODO : Remove optional
		std::optional<Wrapper::Pipeline> _texturedPipeline; //TODO : Remove optional
		std::optional<Wrapper::Queue> _graphicsQueue; //TODO : Remove optional
		std::optional<UploadContext> _uploadContext; //TODO : Remove optional
		std::unordered_map<std::string, std::unique_ptr<RenderObject>> _renderObjects;
		std::unordered_map<std::string, std::shared_ptr<Texture>> _textures;
		std::vector<std::reference_wrapper<RenderObject>> _renderObjectsToDraw;
	};

} // Concerto::Graphics

#endif //CONCERTOGRAPHICS_VULKANRENDERER_HPP
