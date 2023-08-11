//
// Created by arthur on 11/06/22.
//

#ifndef CONCERTOGRAPHICS_SWAPCHAIN_HPP
#define CONCERTOGRAPHICS_SWAPCHAIN_HPP

#include <vector>
#include <optional>
#include <span>

#include <vulkan/vulkan.h>
#include <Concerto/Core/Types.hpp>

#include "Concerto/Graphics/Vulkan/Wrapper/Object.hpp"
#include "Concerto/Graphics/Vulkan/Wrapper/Image.hpp"
#include "Concerto/Graphics/Vulkan/Wrapper/Allocator.hpp"
#include "Concerto/Graphics/Vulkan/Wrapper/Semaphore.hpp"
#include "Concerto/Graphics/Vulkan/Wrapper/Fence.hpp"
#include "Concerto/Graphics/Vulkan/Wrapper/ImageView.hpp"

namespace Concerto::Graphics
{
	class Device;

	class PhysicalDevice;

	/**
	 * @class Swapchain
	 * @brief Represents a swapchain in the vulkan API.
	 * A swapchain is a collection of images that are used for rendering and displaying.
	 */
	class CONCERTO_PUBLIC_API Swapchain : public Object<VkSwapchainKHR>
	{
	public:
		/**
		 * @brief Construct a new Swapchain object
		 * @param device A reference to the logical device.
		 * @param allocator A reference to the allocator.
		 * @param windowExtent The size of the Window.
		 * @param physicalDevice A reference to the physical device.
		 */
		Swapchain(Device& device, Allocator& allocator, VkExtent2D windowExtent, PhysicalDevice& physicalDevice);

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

		/**
		 * @brief Acquire the next image in the swapchain.
		 * @param semaphore A reference to the semaphore.
		 * @param fence A reference to the fence.
		 * @param timeout The timeout.
		 * @return The index of the acquired image.
		 */
		std::uint32_t AcquireNextImage(Semaphore& semaphore, Fence& fence, std::uint64_t timeout);


		private:
			mutable std::optional<std::vector<Image>> _swapChainImages;
			mutable std::optional<std::vector<ImageView>> _swapChainImageViews;
			VkExtent2D _windowExtent;
			VkFormat _swapChainImageFormat;
			Image _depthImage;
			ImageView _depthImageView;
			PhysicalDevice* _physicalDevice;
	};
} // Concerto::Graphics::Wrapper

#endif //CONCERTOGRAPHICS_SWAPCHAIN_HPP
