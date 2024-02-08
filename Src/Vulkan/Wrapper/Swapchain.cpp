//
// Created by arthur on 11/06/22.
//

#include <vulkan/vulkan.h>
#include <Concerto/Core/Assert.hpp>

#include "Concerto/Graphics/Vulkan/Wrapper/Swapchain.hpp"

#include "Fence.hpp"
#include "Concerto/Graphics/Vulkan/Wrapper/PhysicalDevice.hpp"
#include "Concerto/Graphics/Vulkan/Wrapper/Device.hpp"
#include "Concerto/Graphics/Vulkan/Wrapper/RenderPass.hpp"
#include "Concerto/Graphics/Vulkan/Wrapper/Semaphore.hpp"
#include "Concerto/Graphics/Window/Window.hpp"

namespace Concerto::Graphics
{

	Swapchain::Swapchain(Device& device, Window& window)
			: Object<VkSwapchainKHR>(device, [](Device &device, VkSwapchainKHR handle){vkDestroySwapchainKHR(*device.Get(), handle, nullptr);}),
			  _swapChainImages(),
			  _swapChainImageViews(),
			  _windowExtent({window.GetWidth(), window.GetHeight()}),
			  _swapChainImageFormat(VK_FORMAT_B8G8R8A8_SRGB),
			  _depthImage(device, _windowExtent, VK_FORMAT_D32_SFLOAT),
			  _depthImageView(device, _depthImage, VK_IMAGE_ASPECT_DEPTH_BIT),
			  _physicalDevice(device.GetPhysicalDevice()),
			  _window(window),
			  _renderpass(),
			  _frameBuffers()
	{
		PhysicalDevice::SurfaceSupportDetails surfaceSupportDetails = _physicalDevice.GetSurfaceSupportDetails();
		VkSwapchainCreateInfoKHR swapChainCreateInfo{};
		UInt32 imageCount = surfaceSupportDetails.capabilities.minImageCount + 1;
		if (surfaceSupportDetails.capabilities.maxImageCount > 0 &&
			imageCount > surfaceSupportDetails.capabilities.maxImageCount)
			imageCount = surfaceSupportDetails.capabilities.maxImageCount;

		swapChainCreateInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
		swapChainCreateInfo.surface = _physicalDevice.GetSurface();
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
		{
			CONCERTO_ASSERT_FALSE;
			throw std::runtime_error("failed to create swap chain!");
		}
		CreateRenderPass();
		CreateFrameBuffers();
	}

	std::span<Image> Swapchain::GetImages()
	{
		if (_swapChainImages.has_value())
			return _swapChainImages.value();
		UInt32 imageCount;
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

	RenderPass* Swapchain::GetRenderPass()
	{
		if (_renderpass != nullptr)
			return _renderpass.get();
		return nullptr;
	}

	FrameBuffer& Swapchain::GetFrameBuffer(UInt32 index)
	{
		CONCERTO_ASSERT(index < _frameBuffers.size());
		return _frameBuffers[index];
	}

	UInt32 Swapchain::GetCurrentImageIndex() const
	{
		return _currentImageIndex;
	}

	FrameBuffer& Swapchain::GetCurrentFrameBuffer()
	{
		return GetFrameBuffer(_currentImageIndex);
	}

	UInt32 Swapchain::AcquireNextImage(Semaphore& semaphore, Fence& fence, std::uint64_t timeout)
	{
		if (vkAcquireNextImageKHR(*_device->Get(), _handle, timeout, *semaphore.Get(), VK_NULL_HANDLE, &_currentImageIndex) !=
			VK_SUCCESS)
		{
			CONCERTO_ASSERT_FALSE;
			throw std::runtime_error("vkAcquireNextImageKHR fail");
		}
		return _currentImageIndex;
	}

	void Swapchain::CreateRenderPass()
	{
		_renderpass = std::make_unique<RenderPass>(*_device, *this);
	}

	void Swapchain::CreateFrameBuffers()
	{
		CONCERTO_ASSERT(_renderpass != nullptr);
		auto imagesViews = GetImageViews();
		_frameBuffers.reserve(imagesViews.size());
		for (auto& imageView : imagesViews)
			_frameBuffers.emplace_back(*_device, *_renderpass, imageView, _depthImageView, _windowExtent);
	}
}