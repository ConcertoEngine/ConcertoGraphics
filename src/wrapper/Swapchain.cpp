//
// Created by arthur on 11/06/22.
//


#include "wrapper/Swapchain.hpp"
#include "vulkan/vulkan.h"
#include "VkBootstrap.h"
#include "vk_mem_alloc.h"
#include "wrapper/VulkanInitializer.hpp"
#include "wrapper/VulkanInitializer.hpp"

namespace Concerto::Graphics::Wrapper
{

	Swapchain::Swapchain(Allocator& allocator, VkExtent2D windowExtent, VkPhysicalDevice physicalDevice,
			VkDevice device, VkSurfaceKHR surface, VkInstance instance) :
			_allocator(allocator), _windowExtent(windowExtent),
			_physicalDevice(physicalDevice), _device(device),
			_surface(surface), _swapChain(VK_NULL_HANDLE),
			_swapChainImages(),
			_depthImage(windowExtent, VK_FORMAT_D32_SFLOAT, allocator),
			_swapChainImageViews()
	{
		vkb::SwapchainBuilder swapChainBuilder{ _physicalDevice, _device, _surface };
		vkb::Swapchain vkbSwapChain = swapChainBuilder
				.use_default_format_selection()
						//use vsync present mode VK_PRESENT_MODE_FIFO_KHR
				.set_desired_present_mode(VK_PRESENT_MODE_IMMEDIATE_KHR)
				.set_desired_extent(_windowExtent.width, _windowExtent.height)
				.build()
				.value();
		_swapChain = vkbSwapChain.swapchain;
		_swapChainImages = vkbSwapChain.get_images().value();
		_swapChainImageViews = vkbSwapChain.get_image_views().value();
		_swapChainImageFormat = vkbSwapChain.image_format;
		_depthFormat = VK_FORMAT_D32_SFLOAT;

		VkImageViewCreateInfo dview_info = VulkanInitializer::ImageViewCreateInfo(_depthFormat, _depthImage._image,
				VK_IMAGE_ASPECT_DEPTH_BIT);;

		if (vkCreateImageView(_device, &dview_info, nullptr, &_depthImageView) != VK_SUCCESS)
		{
			throw std::runtime_error("Failed to create depth image view");
		}
	}

	Swapchain::~Swapchain()
	{
		vmaDestroyImage(_allocator._allocator, _depthImage._image, _depthImage._allocation);
		vkDestroySwapchainKHR(_device, _swapChain, nullptr);
		_swapChain = VK_NULL_HANDLE;
	}

	std::uint32_t Swapchain::getImageCount() const
	{
		return _swapChainImages.size();
	}

	const std::vector<VkImage>& Swapchain::getImages() const
	{
		return _swapChainImages;
	}

	const std::vector<VkImageView>& Swapchain::getImageViews() const
	{
		return _swapChainImageViews;
	}

	std::uint32_t Swapchain::getImageViewCount() const
	{
		return _swapChainImageViews.size();
	}

	VkExtent2D Swapchain::getExtent() const
	{
		return _windowExtent;
	}

	VkImageView Swapchain::getDepthImageView() const
	{
		return _depthImageView;
	}

	VkFormat Swapchain::getImageFormat() const
	{
		return _swapChainImageFormat;
	}

	VkFormat Swapchain::getDepthFormat() const
	{
		return _depthFormat;
	}

	std::uint32_t Swapchain::acquireNextImage(Semaphore& semaphore, Fence& fence, std::uint64_t timeout)
	{
		std::uint32_t index = 0;
		if (vkAcquireNextImageKHR(_device, _swapChain, timeout, semaphore.Get(), nullptr, &index) != VK_SUCCESS)
		{
			throw std::runtime_error("vkAcquireNextImageKHR fail");
		}
		return index;
	}

	VkSwapchainKHR Swapchain::Get() const
	{
		return _swapChain;
	}
}