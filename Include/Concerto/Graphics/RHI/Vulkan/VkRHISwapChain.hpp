//
// Created by arthur on 12/05/2024.
//

#ifndef CONCERTO_GRAPHICS_BACKEND_RHI_VULKAN_SWAPCHAIN_HPP
#define CONCERTO_GRAPHICS_BACKEND_RHI_VULKAN_SWAPCHAIN_HPP

#include "Concerto/Graphics/RHI/Defines.hpp"

#include "Concerto/Graphics/Backend/Vulkan/Wrapper/CommandBuffer.hpp"
#include "Concerto/Graphics/Backend/Vulkan/Wrapper/Fence.hpp"
#include "Concerto/Graphics/Backend/Vulkan/Wrapper/RenderPass.hpp"
#include "Concerto/Graphics/Backend/Vulkan/Wrapper/Semaphore.hpp"
#include "Concerto/Graphics/Backend/Vulkan/Wrapper/SwapChain.hpp"

#include "Concerto/Graphics/RHI/SwapChain.hpp"
#include "Concerto/Graphics/RHI/Vulkan/VkRHIDevice.hpp"
#include "Concerto/Graphics/RHI/Vulkan/VKRHICommandPool.hpp"

namespace cct::gfx::vk
{
	class Queue;
	class CommandPool;
}

namespace cct::gfx::rhi
{
	class CONCERTO_GRAPHICS_RHI_BASE_API VkRHISwapChain final : public rhi::SwapChain, public vk::SwapChain
	{
	public:
		VkRHISwapChain(rhi::VkRHIDevice& device, Window& window, PixelFormat pixelFormat, PixelFormat depthPixelFormat);
		rhi::RenderPass* GetRenderPass() override;
		Vector2u GetExtent() const override;
		UInt32 GetImageCount() const override;
		Frame& AcquireFrame() override;

		inline VkRHICommandPool& GetCommandPool() const;
		inline VkRHIDevice& GetRHIDevice() const;
		inline vk::Queue& GetPresentQueue() const;
		inline rhi::FrameBuffer& GetCurrentFrameBuffer();
		inline const rhi::FrameBuffer& GetCurrentFrameBuffer() const;

		void Present(UInt32 imageIndex);
	private:
		void CreateFrameBuffers(rhi::VkRHIDevice& device);
		void CreateRenderPass();
		void CreateFrames();

		class SwapChainFrame : public rhi::Frame
		{
		public:
			SwapChainFrame(VkRHISwapChain& owner);

			void Present() override;
			rhi::CommandBuffer& GetCommandBuffer() override;
			std::size_t GetCurrentFrameIndex() override;
			rhi::FrameBuffer& GetFrameBuffer() override;
			void SetNextImageIndex(UInt32 imageIndex);
			void Wait();

			vk::Semaphore& GetPresentSemaphore();
			vk::Semaphore& GetRenderSemaphore();
			vk::Fence& GetRenderFence();

		private:
			std::unique_ptr<rhi::CommandBuffer> _commandBuffer;
			vk::Fence _renderFence;
			vk::Semaphore _presentSemaphore;
			vk::Semaphore _renderSemaphore;
			VkRHISwapChain* _owner = nullptr;
			UInt32 _imageIndex = 0;
		};

		PixelFormat _pixelFormat;
		PixelFormat _depthPixelFormat;

		std::vector<std::unique_ptr<FrameBuffer>> _frameBuffers;
		std::unique_ptr<rhi::RenderPass> _renderPass;
		std::unique_ptr<rhi::CommandPool> _commandPool;
		std::unique_ptr<vk::Queue> _presentQueue;
		std::vector<SwapChainFrame> _frames;

		UInt32 _currentFrameIndex = 0;
		UInt32 _lastFrameIndex = 0;
		bool _needResize = false;
	};
}

#endif //CONCERTO_GRAPHICS_BACKEND_RHI_VULKAN_SWAPCHAIN_HPP