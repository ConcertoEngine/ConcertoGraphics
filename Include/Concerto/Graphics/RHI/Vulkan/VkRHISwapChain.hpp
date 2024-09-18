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

namespace Concerto::Graphics::Vk
{
	class Queue;
	class CommandPool;
}

namespace Concerto::Graphics::RHI
{
	class CONCERTO_GRAPHICS_RHI_BASE_API VkRHISwapChain final : public RHI::SwapChain, public Vk::SwapChain
	{
	public:
		VkRHISwapChain(RHI::VkRHIDevice& device, Window& window, PixelFormat pixelFormat, PixelFormat depthPixelFormat);
		RHI::RenderPass& GetRenderPass() override;
		Vector2u GetExtent() const override;
		UInt32 GetImageCount() const override;
		Frame& AcquireFrame() override;

		inline VkRHICommandPool& GetCommandPool() const;
		inline VkRHIDevice& GetRHIDevice() const;
		inline Vk::Queue& GetPresentQueue() const;
		inline RHI::FrameBuffer& GetCurrentFrameBuffer();
		inline const RHI::FrameBuffer& GetCurrentFrameBuffer() const;

		void Present(UInt32 imageIndex);
	private:
		void CreateFrameBuffers(RHI::VkRHIDevice& device);
		void CreateRenderPass();
		void CreateFrames();

		class SwapChainFrame : public RHI::Frame
		{
		public:
			SwapChainFrame(VkRHISwapChain& owner);

			void Present() override;
			RHI::CommandBuffer& GetCommandBuffer() override;
			std::size_t GetCurrentFrameIndex() override;
			RHI::FrameBuffer& GetFrameBuffer() override;
			void SetNextImageIndex(UInt32 imageIndex);
			void Wait();

			Vk::Semaphore& GetPresentSemaphore();
			Vk::Semaphore& GetRenderSemaphore();
			Vk::Fence& GetRenderFence();

		private:
			std::unique_ptr<RHI::CommandBuffer> _commandBuffer;
			Vk::Fence _renderFence;
			Vk::Semaphore _presentSemaphore;
			Vk::Semaphore _renderSemaphore;
			VkRHISwapChain* _owner = nullptr;
			UInt32 _imageIndex = 0;
		};

		PixelFormat _pixelFormat;
		PixelFormat _depthPixelFormat;

		std::vector<SwapChainFrame> _frames;
		std::vector<std::unique_ptr<FrameBuffer>> _frameBuffers;
		std::unique_ptr<RHI::RenderPass> _renderPass;
		std::unique_ptr<RHI::CommandPool> _commandPool;
		std::unique_ptr<Vk::Queue> _presentQueue;

		UInt32 _currentFrameIndex = 0;
		bool _needResize = false;
	};
}

#endif //CONCERTO_GRAPHICS_BACKEND_RHI_VULKAN_SWAPCHAIN_HPP