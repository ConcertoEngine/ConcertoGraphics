//
// Created by arthur on 11/06/22.
//

#include <cassert>

#include "wrapper/Swapchain.hpp"
#include "vulkan/vulkan.h"
#include "vk_mem_alloc.h"
#include "wrapper/VulkanInitializer.hpp"
#include "wrapper/PhysicalDevice.hpp"
#include "wrapper/Device.hpp"
#include "wrapper/ImageView.hpp"

namespace Concerto::Graphics::Wrapper
{

	Swapchain::Swapchain(Device& device, Allocator& allocator, VkExtent2D windowExtent, PhysicalDevice& physicalDevice)
			: Object<VkSwapchainKHR>(device, [this](){vkDestroySwapchainKHR(*_device->Get(), _handle, nullptr);}),
			  _windowExtent(windowExtent),
			  _swapChainImages(),
			  _depthImage(device, windowExtent, VK_FORMAT_D32_SFLOAT, allocator),
			  _depthImageView(device, _depthImage, VK_IMAGE_ASPECT_DEPTH_BIT),
			  _swapChainImageViews(),
			  _swapChainImageFormat(VK_FORMAT_B8G8R8A8_SRGB)
	{
		PhysicalDevice::SurfaceSupportDetails surfaceSupportDetails = physicalDevice.GetSurfaceSupportDetails();
		VkSwapchainCreateInfoKHR swapChainCreateInfo{};
		std::uint32_t imageCount = surfaceSupportDetails.capabilities.minImageCount + 1;
		if (surfaceSupportDetails.capabilities.maxImageCount > 0 &&
			imageCount > surfaceSupportDetails.capabilities.maxImageCount)
			imageCount = surfaceSupportDetails.capabilities.maxImageCount;

		swapChainCreateInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
		swapChainCreateInfo.surface = physicalDevice.GetSurface();
		swapChainCreateInfo.minImageCount = imageCount;
		swapChainCreateInfo.imageFormat = _swapChainImageFormat;
		swapChainCreateInfo.imageColorSpace = VK_COLOR_SPACE_SRGB_NONLINEAR_KHR;
		swapChainCreateInfo.imageExtent = _windowExtent;
		swapChainCreateInfo.imageArrayLayers = 1;
		swapChainCreateInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
		swapChainCreateInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
		swapChainCreateInfo.preTransform = VK_SURFACE_TRANSFORM_IDENTITY_BIT_KHR;
		swapChainCreateInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
		swapChainCreateInfo.presentMode = VK_PRESENT_MODE_IMMEDIATE_KHR;
		swapChainCreateInfo.clipped = VK_TRUE;
		swapChainCreateInfo.oldSwapchain = VK_NULL_HANDLE;
		if (vkCreateSwapchainKHR(*_device->Get(), &swapChainCreateInfo, nullptr, &_handle) != VK_SUCCESS)
			throw std::runtime_error("failed to create swap chain!");
	}

	std::span<Image> Swapchain::GetImages()
	{
		if (_swapChainImages.has_value())
			return _swapChainImages.value();
		std::uint32_t imageCount;
		std::vector<VkImage> swapChainImages;
		vkGetSwapchainImagesKHR(*_device->Get(), _handle, &imageCount, nullptr);
		swapChainImages.resize(imageCount);
		vkGetSwapchainImagesKHR(*_device->Get(), _handle, &imageCount, swapChainImages.data());
		std::vector<Image> images;
		images.reserve(imageCount);
		for (auto& image: swapChainImages)
			images.emplace_back(*_device, image, _swapChainImageFormat);
		_swapChainImages = std::move(images);
		return _swapChainImages.value();
	}

	std::span<ImageView> Swapchain::GetImageViews()
	{
		if (_swapChainImageViews)
			return _swapChainImageViews.value();
		std::vector<ImageView> swapChainImageViews;
		swapChainImageViews.reserve(GetImages().size());
		for (auto& image: GetImages())
			swapChainImageViews.emplace_back(*_device, image, VK_IMAGE_ASPECT_COLOR_BIT);
		_swapChainImageViews = std::move(swapChainImageViews);
		return _swapChainImageViews.value();
	}

	VkExtent2D Swapchain::GetExtent() const
	{
		return _windowExtent;
	}

	const ImageView& Swapchain::GetDepthImageView() const
	{
		return _depthImageView;
	}

	ImageView& Swapchain::GetDepthImageView()
	{
		return _depthImageView;
	}

	VkFormat Swapchain::GetImageFormat() const
	{

		return _swapChainImageFormat;
	}

	VkFormat Swapchain::GetDepthFormat() const
	{
		return _depthImage.GetFormat();
	}

	std::uint32_t Swapchain::AcquireNextImage(Semaphore& semaphore, Fence& fence, std::uint64_t timeout)
	{
		std::uint32_t index = 0;
		if (vkAcquireNextImageKHR(*_device->Get(), _handle, timeout, *semaphore.Get(), VK_NULL_HANDLE, &index) !=
			VK_SUCCESS)
		{
			throw std::runtime_error("vkAcquireNextImageKHR fail");
		}
		return index;
	}
}