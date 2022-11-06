//
// Created by arthur on 11/06/22.
//

#ifndef CONCERTOGRAPHICS_SWAPCHAIN_HPP
#define CONCERTOGRAPHICS_SWAPCHAIN_HPP

#include <vector>
#include <optional>
#include <span>
#include "vulkan/vulkan.h"
#include "wrapper/Object.hpp"
#include "Image.hpp"
#include "Allocator.hpp"
#include "Semaphore.hpp"
#include "Fence.hpp"
#include "ImageView.hpp"

namespace Concerto::Graphics::Wrapper
{
	class Device;

	class PhysicalDevice;

	class Swapchain : public Object<VkSwapchainKHR>
	{
	public:
		Swapchain(Device& device, Allocator& allocator, VkExtent2D windowExtent, PhysicalDevice& physicalDevice);

		Swapchain(Swapchain&&) noexcept = default;

		Swapchain(const Swapchain&) = delete;

		Swapchain& operator=(Swapchain&&) noexcept = default;

		Swapchain& operator=(const Swapchain&) = delete;

		[[nodiscard]] std::span<Image> GetImages();

		[[nodiscard]] std::span<ImageView> GetImageViews();

		[[nodiscard]] VkExtent2D GetExtent() const;

		[[nodiscard]] const ImageView& GetDepthImageView() const;

		[[nodiscard]] ImageView& GetDepthImageView();

		[[nodiscard]] VkFormat GetImageFormat() const;

		[[nodiscard]] VkFormat GetDepthFormat() const;

		std::uint32_t AcquireNextImage(Semaphore& semaphore, Fence& fence, std::uint64_t timeout);

	private:
		mutable std::optional<std::vector<Image>> _swapChainImages;
		mutable std::optional<std::vector<ImageView>> _swapChainImageViews;
		VkExtent2D _windowExtent;
		VkFormat _swapChainImageFormat;
		Image _depthImage;
		ImageView _depthImageView;
	};

} // Concerto::Graphics::Wrapper

#endif //CONCERTOGRAPHICS_SWAPCHAIN_HPP
