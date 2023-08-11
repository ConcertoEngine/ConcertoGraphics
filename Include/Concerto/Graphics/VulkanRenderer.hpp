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
#include <chrono>

#include <Concerto/Core/Math/Vector.hpp>
#include <Concerto/Core/Math/Transform.hpp>
#include <Concerto/Core/Types.hpp>

#include "Concerto/Graphics/Vulkan/Wrapper/Instance.hpp"
#include "Concerto/Graphics/Vulkan/Wrapper/Device.hpp"
#include "Concerto/Graphics/Vulkan/Wrapper/PhysicalDevice.hpp"
#include "Concerto/Graphics/Vulkan/Wrapper/Allocator.hpp"
#include "Concerto/Graphics/Vulkan/Wrapper/Allocator.hpp"
#include "Concerto/Graphics/Vulkan/Wrapper/Swapchain.hpp"
#include "Concerto/Graphics/Vulkan/Wrapper/RenderPass.hpp"
#include "Concerto/Graphics/Vulkan/Wrapper/FrameBuffer.hpp"
#include "Concerto/Graphics/Vulkan/Wrapper/DescriptorSetLayout.hpp"
#include "Concerto/Graphics/Vulkan/Wrapper/DescriptorPool.hpp"
#include "Concerto/Graphics/Vulkan/Wrapper/Buffer.hpp"
#include "Concerto/Graphics/Vulkan/Wrapper/ShaderModule.hpp"
#include "Concerto/Graphics/Vulkan/Wrapper/PipelineLayout.hpp"
#include "Concerto/Graphics/Vulkan/Wrapper/Pipeline.hpp"
#include "Concerto/Graphics/Vulkan/Wrapper/Queue.hpp"
#include "Concerto/Graphics/Vulkan/Wrapper/Sampler.hpp"
#include "Concerto/Graphics/Vulkan/DescriptorLayoutCache.hpp"
#include "Concerto/Graphics/Vulkan/DescriptorAllocator.hpp"
#include "Concerto/Graphics/Window/GlfW3.hpp"
#include "Concerto/Graphics/Version.hpp"
#include "Concerto/Graphics/Frame.hpp"
#include "Concerto/Graphics/UploadContext.hpp"
#include "Concerto/Graphics/Vulkan/RenderObject.hpp"
#include "Concerto/Graphics/Vulkan/Texture.hpp"
#include "Concerto/Graphics/ImGUI.hpp"
#include "Concerto/Graphics/TextureBuilder.hpp"
#include "Concerto/Graphics/MaterialBuilder.hpp"
#include "Concerto/Graphics/Mesh.hpp"
#include "Concerto/Graphics/Vulkan/VkMesh.hpp"
#include "Concerto/Graphics/Camera.hpp"

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

	class CONCERTO_PUBLIC_API VulkanRenderer
	{
	 public:
		explicit VulkanRenderer(RendererInfo info, GlfW3& window);

		~VulkanRenderer() = default;

		VulkanRenderer(const VulkanRenderer&) = delete;

		VulkanRenderer(VulkanRenderer&&) = delete;

		VulkanRenderer& operator=(const VulkanRenderer&) = delete;

		VulkanRenderer& operator=(VulkanRenderer&&) = delete;

		VulkanRenderer* Instance();

		void Draw(const GPUCamera& camera);
		void DrawObject(MeshPtr &mesh, const Math::Transform transform);
		void DrawObject(MeshPtr &mesh,
			const Math::Vector3f& position,
			const Math::Quaternionf& rotation,
			const Math::Vector3f& scale);
		void UseImGUI();
		ImGUI* GetImGUIContext();
		void UpdateSceneParameters(const Scene& sceneData);
		void Resize(std::uint32_t width, std::uint32_t height);
	 private:
		VkMeshPtr LoadModelIfNotExist(MeshPtr &mesh);
		void DrawObjects(const GPUCamera& camera);
		std::vector<const char*> _deviceExtensions = { VK_KHR_SWAPCHAIN_EXTENSION_NAME };
		GlfW3& _window;
		RendererInfo _renderInfo;
		Scene _sceneParameters;
		VulkanRenderer* _instance{ nullptr };
		std::uint32_t _frameNumber{ 0 };
		Graphics::Instance _vulkanInstance;
		VkSurfaceKHR _surface{ VK_NULL_HANDLE };
		PhysicalDevice _physicalDevice;
		Device _device;
		bool _isResized{ false };
		VkPhysicalDeviceProperties _gpuProperties;
		std::uint32_t _graphicsQueueFamilyIndex;
		std::unique_ptr<Graphics::ImGUI> _imGUI;
		std::unique_ptr<Allocator> _allocator;
		std::unique_ptr<Swapchain> _swapchain;
		std::unique_ptr<RenderPass> _renderPass;
		std::vector<FrameBuffer> _frameBuffers;
		std::unique_ptr<DescriptorSetLayout> _globalSetLayout;
		std::unique_ptr<DescriptorSetLayout> _objectSetLayout;
		std::unique_ptr<DescriptorSetLayout> _singleTextureSetLayout;
		std::unique_ptr<DescriptorPool> _descriptorPool;
		std::vector<FrameData> _frames;
		std::unique_ptr<Buffer> _sceneParameterBuffer;
		//TODO : Convert all Shaders modules into an unordered_map
		std::unique_ptr<ShaderModule> _colorMeshShader;
		std::unique_ptr<ShaderModule> _textureMeshShader;
		std::unique_ptr<ShaderModule> _meshVertShader;
		std::unique_ptr<PipelineLayout> _meshPipelineLayout;
		std::unique_ptr<PipelineLayout> _texturedSetLayout;
		std::unique_ptr<Pipeline> _coloredShaderPipeline;
		std::unique_ptr<Pipeline> _texturedPipeline;
		std::unique_ptr<Queue> _graphicsQueue;
		std::unique_ptr<UploadContext> _uploadContext;
		std::unordered_map<std::string, RenderObjectPtr> _renderObjects;
		std::unordered_map<std::string, VkMeshPtr> _meshes;
		std::unordered_map<std::string, std::shared_ptr<Texture>> _textures;
		std::unordered_map<VkMeshPtr, std::vector<glm::mat4>> _renderObjectsToDraw;
		std::unique_ptr<TextureBuilder> _textureBuilder;
		std::unique_ptr<DescriptorLayoutCache> _descriptorLayoutCache;
		std::unique_ptr<DescriptorAllocator> _descriptorAllocator;
		std::unique_ptr<MaterialBuilder> _materialBuilder;
		std::unique_ptr<Sampler> _sampler;

	 public:
		//timings
		float _transfersTime{ 0 };
		float _drawTime{ 0 };
		float _drawObjectsTime{ 0 };
	};

} // Concerto::Graphics

#endif //CONCERTOGRAPHICS_VULKANRENDERER_HPP
