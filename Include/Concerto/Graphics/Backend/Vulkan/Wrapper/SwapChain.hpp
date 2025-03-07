//
// Created by arthur on 11/06/22.
//

#ifndef CONCERTO_GRAPHICS_SWAPCHAIN_HPP
#define CONCERTO_GRAPHICS_SWAPCHAIN_HPP

#include <vector>
#include <optional>
#include <span>


#include "Concerto/Graphics/Backend/Vulkan/Defines.hpp"

#include "Concerto/Graphics/Backend/Vulkan/Wrapper/Object.hpp"
#include "Concerto/Graphics/Backend/Vulkan/Wrapper/Image.hpp"
#include "Concerto/Graphics/Backend/Vulkan/Wrapper/ImageView.hpp"
#include "Concerto/Graphics/Backend/Vulkan/Wrapper/FrameBuffer.hpp"

namespace cct::gfx
{
	class Window;
}

namespace cct::gfx::vk
{
	class Device;
	class PhysicalDevice;
	class RenderPass;
	class Fence;
	class Semaphore;

	/**
	 * @class SwapChain
	 * @brief Represents a SwapChain in the vulkan API.
	 * A SwapChain is a collection of images that are used for rendering and displaying.
	 */
	class CONCERTO_GRAPHICS_VULKAN_BACKEND_API SwapChain : public Object<VkSwapchainKHR>
	{
	public:
		SwapChain(Device& device, Window& window, VkFormat colorFormat = VK_FORMAT_B8G8R8A8_SRGB, VkFormat depthFormat = VK_FORMAT_D32_SFLOAT);
		~SwapChain();
		SwapChain(SwapChain&&) noexcept = default;
		SwapChain(const SwapChain&) = delete;
		SwapChain& operator=(SwapChain&&) noexcept = delete;
		SwapChain& operator=(const SwapChain&) = delete;

		/**
		 * @brief Get the images in the swapChain.
		 * @return A span of Image objects.
		 */
		[[nodiscard]] std::span<Image> GetImages() const;

		[[nodiscard]] std::span<ImageView> GetImageViews();

		[[nodiscard]] VkExtent2D GetExtent() const;

		[[nodiscard]] const ImageView& GetDepthImageView() const;

		[[nodiscard]] ImageView& GetDepthImageView();

		[[nodiscard]] VkFormat GetImageFormat() const;

		[[nodiscard]] VkFormat GetDepthFormat() const;

		[[nodiscard]] FrameBuffer& GetFrameBuffer(UInt32 index);

		[[nodiscard]] UInt32 GetCurrentImageIndex() const;

		/**
		 * @brief Acquire the next image in the swapchain.
		 * @param semaphore A reference to the semaphore.
		 * @param fence A reference to the fence.
		 * @param timeout The timeout.
		 * @return The index of the acquired image.
		 */
		UInt32 AcquireNextImage(const Semaphore& semaphore, const Fence* fence = nullptr, UInt64 timeout = std::numeric_limits<UInt64>::max());

		bool ReCreate(VkFormat pixelFormat = VK_FORMAT_B8G8R8A8_SRGB, VkFormat depthFormat = VK_FORMAT_D32_SFLOAT);

	private:
		void CreateFrameBuffers();

		mutable std::optional<std::vector<Image>> _swapChainImages;
		mutable std::optional<std::vector<ImageView>> _swapChainImageViews;
		VkExtent2D _windowExtent;
		VkFormat _swapChainImageFormat;
		Image _depthImage;
		ImageView _depthImageView;
		PhysicalDevice& _physicalDevice;
		Window& _window;
		UInt32 _currentImageIndex;
		VkSurfaceKHR _surface;
	};
} // cct::gfx::vk

#endif //CONCERTO_GRAPHICS_SWAPCHAIN_HPP
