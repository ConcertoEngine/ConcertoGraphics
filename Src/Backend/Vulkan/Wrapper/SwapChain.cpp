//
// Created by arthur on 11/06/22.
//


#include <Concerto/Core/Assert.hpp>

#include "Concerto/Graphics/Backend/Vulkan/Wrapper/Swapchain.hpp"

#include "Concerto/Graphics/Backend/Vulkan/Wrapper/Fence.hpp"
#include "Concerto/Graphics/Backend/Vulkan/Wrapper/PhysicalDevice.hpp"
#include "Concerto/Graphics/Backend/Vulkan/Wrapper/Device.hpp"
#include "Concerto/Graphics/Backend/Vulkan/Wrapper/RenderPass.hpp"
#include "Concerto/Graphics/Backend/Vulkan/Wrapper/Semaphore.hpp"
#include "Concerto/Graphics/Window/Window.hpp"
#include "Concerto/Graphics/Backend/Vulkan/Wrapper/Instance.hpp"

namespace cct::gfx::vk
{
	SwapChain::SwapChain(Device& device, Window& window, VkFormat colorFormat, VkFormat depthFormat) : Object(device),
		_swapChainImages(),
		_swapChainImageViews(),
		_windowExtent({window.GetWidth(), window.GetHeight()}),
		_swapChainImageFormat(colorFormat),
		_depthImage(device, _windowExtent, depthFormat),
		_depthImageView(device, _depthImage, VK_IMAGE_ASPECT_DEPTH_BIT),
		_physicalDevice(device.GetPhysicalDevice()),
		_window(window),
		_currentImageIndex(0),
		_surface(VK_NULL_HANDLE)
	{
		this->ReCreate(colorFormat, depthFormat);
	}

	SwapChain::~SwapChain()
	{
		if (IsNull())
			return;
		_device->vkDestroySwapchainKHR(*_device->Get(), _handle, nullptr);
	}

	std::span<Image> SwapChain::GetImages() const
	{
		if (_swapChainImages.has_value())
			return _swapChainImages.value();
		UInt32 imageCount;
		std::vector<VkImage> swapChainImages;
		_device->vkGetSwapchainImagesKHR(*_device->Get(), _handle, &imageCount, nullptr);
		swapChainImages.resize(imageCount);
		_device->vkGetSwapchainImagesKHR(*_device->Get(), _handle, &imageCount, swapChainImages.data());
		std::vector<Image> images;
		images.reserve(imageCount);
		for (auto& image: swapChainImages)
			images.emplace_back(*_device, _windowExtent, image, _swapChainImageFormat);
		_swapChainImages = std::move(images);
		return _swapChainImages.value();
	}

	std::span<ImageView> SwapChain::GetImageViews()
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

	VkExtent2D SwapChain::GetExtent() const
	{
		return _windowExtent;
	}

	const ImageView& SwapChain::GetDepthImageView() const
	{
		return _depthImageView;
	}

	ImageView& SwapChain::GetDepthImageView()
	{
		return _depthImageView;
	}

	VkFormat SwapChain::GetImageFormat() const
	{

		return _swapChainImageFormat;
	}

	VkFormat SwapChain::GetDepthFormat() const
	{
		return _depthImage.GetFormat();
	}

	UInt32 SwapChain::GetCurrentImageIndex() const
	{
		return _currentImageIndex;
	}

	UInt32 SwapChain::AcquireNextImage(Semaphore& semaphore, Fence& fence, std::uint64_t timeout)
	{
		_lastResult = _device->vkAcquireNextImageKHR(*_device->Get(), _handle, timeout, *semaphore.Get(), VK_NULL_HANDLE, &_currentImageIndex);
		if (_lastResult != VK_SUCCESS)
		{
			CCT_ASSERT_FALSE("ConcertoGraphics: vkCreateSemaphore failed VKResult={}", static_cast<int>(_lastResult));
			return std::numeric_limits<UInt32>::max();
		}
		return _currentImageIndex;
	}

	bool SwapChain::ReCreate(VkFormat pixelFormat, VkFormat depthFormat)
	{
		_windowExtent = { _window.GetWidth(), _window.GetHeight() };
		if (_handle)
			_device->vkDestroySwapchainKHR(*_device->Get(), _handle, nullptr);
		if (_surface)
			_device->GetInstance().vkDestroySurfaceKHR(*GetDevice()->GetInstance().Get(), _surface, nullptr);
		NativeWindow nativeWindow = _window.GetNativeWindow();
#if defined(CCT_PLATFORM_WINDOWS)
		const VkWin32SurfaceCreateInfoKHR createInfo = {
			.sType = VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR,
			.pNext = nullptr,
			.flags = 0,
			.hinstance = static_cast<HINSTANCE>(nativeWindow.hinstance),
			.hwnd = static_cast<HWND>(nativeWindow.window)
		};
		VkResult result = _device->GetInstance().vkCreateWin32SurfaceKHR(*_device->GetInstance().Get(), &createInfo, nullptr, &_surface);
#elif defined(CCT_PLATFORM_MACOS))
		CCT_ASSERT_FALSE("Not implemented");
#elif defined(CCT_PLATFORM_LINUX)
		CCT_ASSERT_FALSE("Not implemented");
#endif
		if (result != VK_SUCCESS)
		{
			CCT_ASSERT_FALSE("ConcertoGraphics: could not create vulkan window surface");
			return false;
		}
		_swapChainImageViews.reset();
		_swapChainImages.reset();
		_depthImage = Image(*GetDevice(), _windowExtent, depthFormat);
		_depthImageView = ImageView(*GetDevice(), _depthImage, VK_IMAGE_ASPECT_DEPTH_BIT);
		PhysicalDevice::SurfaceSupportDetails surfaceSupportDetails = _physicalDevice.GetSurfaceSupportDetails(_surface);
		VkSwapchainCreateInfoKHR swapChainCreateInfo = {};
		UInt32 imageCount = surfaceSupportDetails.capabilities.minImageCount + 1;
		if (surfaceSupportDetails.capabilities.maxImageCount > 0 && imageCount > surfaceSupportDetails.capabilities.maxImageCount)
			imageCount = surfaceSupportDetails.capabilities.maxImageCount;

		swapChainCreateInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
		swapChainCreateInfo.surface = _surface;
		swapChainCreateInfo.minImageCount = imageCount;
		swapChainCreateInfo.imageFormat = pixelFormat;
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
		_lastResult = _device->vkCreateSwapchainKHR(*_device->Get(), &swapChainCreateInfo, nullptr, &_handle);
		if (_lastResult != VK_SUCCESS)
		{
			CCT_ASSERT_FALSE("ConcertoGraphics: vkCreateSwapchainKHR failed VKResult={}",
								  static_cast<int>(_lastResult));
			return false;
		}
		return true;
	}
}
