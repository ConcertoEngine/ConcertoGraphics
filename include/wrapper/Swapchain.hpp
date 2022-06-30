//
// Created by arthur on 11/06/22.
//

#ifndef CONCERTOGRAPHICS_SWAPCHAIN_HPP
#define CONCERTOGRAPHICS_SWAPCHAIN_HPP

#include <vector>
#include "vulkan/vulkan.h"
#include "AllocatedImage.hpp"
#include "Allocator.hpp"

namespace Concerto::Graphics::Wrapper
{
	class Swapchain
	{
	public:
		Swapchain(Allocator& allocator, VkExtent2D windowExtent, VkPhysicalDevice physicalDevice, VkDevice device,
				VkSurfaceKHR surface, VkInstance instance);

		Swapchain(Swapchain&&) = default;

		Swapchain(const Swapchain&) = delete;

		Swapchain& operator=(Swapchain&&) = default;

		Swapchain& operator=(const Swapchain&) = delete;

		~Swapchain();

		[[nodiscard]] std::uint32_t getImageCount() const;

		[[nodiscard]] const std::vector<VkImage>& getImages() const;

		[[nodiscard]] const std::vector<VkImageView>& getImageViews() const;

		[[nodiscard]] std::uint32_t getImageViewCount() const;

		VkExtent2D getExtent() const;

		VkImageView getDepthImageView() const;

		VkFormat getImageFormat() const;

		VkFormat getDepthFormat() const;

	private:
		Allocator &_allocator;
		VkPhysicalDevice _physicalDevice;
		VkDevice _device;
		VkSurfaceKHR _surface;
		VkExtent2D _windowExtent;
		VkSwapchainKHR _swapChain{};
		std::vector<VkImage> _swapChainImages;
		std::vector<VkImageView> _swapChainImageViews;
		VkFormat _swapChainImageFormat;
		VkFormat _depthFormat;
		AllocatedImage _depthImage{};
		VkImageView _depthImageView{};
	};

} // Concerto::Graphics::Wrapper

#endif //CONCERTOGRAPHICS_SWAPCHAIN_HPP
