//
// Created by arthur on 11/06/22.
//

#define VMA_IMPLEMENTATION
#include "wrapper/Swapchain.hpp"
#include "VkBootstrap.h"
#include "vk_mem_alloc.h"
#include "wrapper/VulkanInitializer.hpp"
namespace Concerto::Graphics::Wrapper
{

	Swapchain::Swapchain(Allocator& allocator, VkExtent2D windowExtent, VkPhysicalDevice physicalDevice, VkDevice device,
			VkSurfaceKHR surface, VkInstance instance) : _windowExtent(windowExtent), _physicalDevice(physicalDevice), _device(device),
									_surface(surface), _swapChain(VK_NULL_HANDLE), _swapChainImages(), _swapChainImageViews()
	{
		vkb::SwapchainBuilder swapChainBuilder{ _physicalDevice, _device, _surface };
		vkb::Swapchain vkbSwapChain = swapChainBuilder
				.use_default_format_selection()
						//use vsync present mode
				.set_desired_present_mode(VK_PRESENT_MODE_FIFO_KHR)
				.set_desired_extent(_windowExtent.width, _windowExtent.height)
				.build()
				.value();
		_swapChain = vkbSwapChain.swapchain;
		_swapChainImages = vkbSwapChain.get_images().value();
		_swapChainImageViews = vkbSwapChain.get_image_views().value();
		_swapChainImageFormat = vkbSwapChain.image_format;

		VkExtent3D depthImageExtent = {
				_windowExtent.width,
				_windowExtent.height,
				1
		};

		_depthFormat = VK_FORMAT_D32_SFLOAT;

		VkImageCreateInfo dimg_info = VulkanInitializer::ImageCreateInfo(_depthFormat,
				VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT, depthImageExtent);

		VmaAllocationCreateInfo dimg_allocinfo = {};
		dimg_allocinfo.usage = VMA_MEMORY_USAGE_GPU_ONLY;
		dimg_allocinfo.requiredFlags = VkMemoryPropertyFlags(VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);

		vmaCreateImage(allocator._allocator, &dimg_info, &dimg_allocinfo, &_depthImage._image, &_depthImage._allocation, nullptr);

		VkImageViewCreateInfo dview_info = VulkanInitializer::ImageViewCreateInfo(_depthFormat, _depthImage._image,
				VK_IMAGE_ASPECT_DEPTH_BIT);;

		if(vkCreateImageView(_device, &dview_info, nullptr, &_depthImageView) != VK_SUCCESS)
		{
			throw std::runtime_error("Failed to create depth image view");
		}
	}

	Swapchain::~Swapchain()
	{
		vkDestroySwapchainKHR(_device, _swapChain, nullptr);
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
}