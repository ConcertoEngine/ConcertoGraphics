//
// Created by arthur on 11/06/22.
//

#ifndef CONCERTOGRAPHICS_SWAPCHAIN_HPP
#define CONCERTOGRAPHICS_SWAPCHAIN_HPP

#include <vector>
#include <optional>
#include <span>

#include <vulkan/vulkan.h>
#include "Concerto/Graphics/Defines.hpp"

#include "Concerto/Graphics/Vulkan/Wrapper/Object.hpp"
#include "Concerto/Graphics/Vulkan/Wrapper/Image.hpp"
#include "Concerto/Graphics/Vulkan/Wrapper/Allocator.hpp"
#include "Concerto/Graphics/Vulkan/Wrapper/Semaphore.hpp"
#include "Concerto/Graphics/Vulkan/Wrapper/Fence.hpp"
#include "Concerto/Graphics/Vulkan/Wrapper/ImageView.hpp"
#include "Concerto/Graphics/Vulkan/Wrapper/RenderPass.hpp"
#include "Concerto/Graphics/Vulkan/Wrapper/FrameBuffer.hpp"
#include "Concerto/Graphics/Window/GlfW3.hpp"

namespace Concerto::Graphics
{
	class Device;

	class PhysicalDevice;

	/**
	 * @class Swapchain
	 * @brief Represents a swapchain in the vulkan API.
	 * A swapchain is a collection of images that are used for rendering and displaying.
	 */
	class CONCERTO_GRAPHICS_API Swapchain : public Object<VkSwapchainKHR>
	{
	public:
		/**
		 * @brief Construct a new Swapchain object
		 * @param device A reference to the logical device.
		 * @param allocator A reference to the allocator.
		 * @param windowExtent The size of the Window.
		 * @param physicalDevice A reference to the physical device.
		 */
		Swapchain(Device& device, GlfW3& window);

		Swapchain(Swapchain&&) noexcept = default;

		Swapchain(const Swapchain&) = delete;

		Swapchain& operator=(Swapchain&&) noexcept = default;

		Swapchain& operator=(const Swapchain&) = delete;

		/**
		 * @brief Get the images in the swapchain.
		 * @return A span of Image objects.
		 */
		[[nodiscard]] std::span<Image> GetImages();

		[[nodiscard]] std::span<ImageView> GetImageViews();

		[[nodiscard]] VkExtent2D GetExtent() const;

		[[nodiscard]] const ImageView& GetDepthImageView() const;

		[[nodiscard]] ImageView& GetDepthImageView();

		[[nodiscard]] VkFormat GetImageFormat() const;

		[[nodiscard]] VkFormat GetDepthFormat() const;

		[[nodiscard]] RenderPass* GetRenderPass();
		
		[[nodiscard]] FrameBuffer& GetFrameBuffer(std::size_t index);
		/**
		 * @brief Acquire the next image in the swapchain.
		 * @param semaphore A reference to the semaphore.
		 * @param fence A reference to the fence.
		 * @param timeout The timeout.
		 * @return The index of the acquired image.
		 */
		UInt32 AcquireNextImage(Semaphore& semaphore, Fence& fence, std::uint64_t timeout);
		
		private:
			void CreateRenderPass();
			void CreateFrameBuffers();

			mutable std::optional<std::vector<Image>> _swapChainImages;
			mutable std::optional<std::vector<ImageView>> _swapChainImageViews;
			VkExtent2D _windowExtent;
			VkFormat _swapChainImageFormat;
			Image _depthImage;
			ImageView _depthImageView;
			PhysicalDevice& _physicalDevice;
			GlfW3& _window;
			std::optional<RenderPass> _renderpass;
			std::vector<FrameBuffer> _frameBuffers;
	};
} // Concerto::Graphics::Wrapper

#endif //CONCERTOGRAPHICS_SWAPCHAIN_HPP
