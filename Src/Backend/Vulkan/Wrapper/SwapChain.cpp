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
		m_swapChainImages(),
		m_swapChainImageViews(),
		m_windowExtent({window.GetWidth(), window.GetHeight()}),
		m_swapChainImageFormat(colorFormat),
		m_depthImage(device.GetAllocator().AllocateImage(m_windowExtent, depthFormat, VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT)),
		m_depthImageView(device, m_depthImage, VK_IMAGE_ASPECT_DEPTH_BIT),
		m_physicalDevice(device.GetPhysicalDevice()),
		m_window(window),
		m_currentImageIndex(0),
		m_surface(VK_NULL_HANDLE)
	{
		this->ReCreate(colorFormat, depthFormat);
	}

	SwapChain::~SwapChain()
	{
		if (IsNull())
			return;
		m_device->vkDestroySwapchainKHR(*m_device->Get(), m_handle, nullptr);
	}

	std::span<Image> SwapChain::GetImages() const
	{
		if (m_swapChainImages.has_value())
			return m_swapChainImages.value();
		UInt32 imageCount;
		std::vector<VkImage> swapChainImages;
		m_device->vkGetSwapchainImagesKHR(*m_device->Get(), m_handle, &imageCount, nullptr);
		swapChainImages.resize(imageCount);
		m_device->vkGetSwapchainImagesKHR(*m_device->Get(), m_handle, &imageCount, swapChainImages.data());
		std::vector<Image> images;
		images.reserve(imageCount);
		for (auto& image: swapChainImages)
			images.emplace_back(m_device->GetAllocator(), m_windowExtent, image, m_swapChainImageFormat);
		m_swapChainImages = std::move(images);
		return m_swapChainImages.value();
	}

	std::span<ImageView> SwapChain::GetImageViews()
	{
		if (m_swapChainImageViews)
			return m_swapChainImageViews.value();
		std::vector<ImageView> swapChainImageViews;
		swapChainImageViews.reserve(GetImages().size());
		for (auto& image: GetImages())
			swapChainImageViews.emplace_back(*m_device, image, VK_IMAGE_ASPECT_COLOR_BIT);
		m_swapChainImageViews = std::move(swapChainImageViews);
		return m_swapChainImageViews.value();
	}

	VkExtent2D SwapChain::GetExtent() const
	{
		return m_windowExtent;
	}

	const ImageView& SwapChain::GetDepthImageView() const
	{
		return m_depthImageView;
	}

	ImageView& SwapChain::GetDepthImageView()
	{
		return m_depthImageView;
	}

	VkFormat SwapChain::GetImageFormat() const
	{

		return m_swapChainImageFormat;
	}

	VkFormat SwapChain::GetDepthFormat() const
	{
		return m_depthImage.GetFormat();
	}

	UInt32 SwapChain::GetCurrentImageIndex() const
	{
		return m_currentImageIndex;
	}

	UInt32 SwapChain::AcquireNextImage(const Semaphore& semaphore, const Fence* fence, UInt64 timeout)
	{
		m_lastResult = m_device->vkAcquireNextImageKHR(*m_device->Get(), m_handle, timeout, *semaphore.Get(), fence ? *fence->Get() : nullptr, &m_currentImageIndex);
		if (m_lastResult != VK_SUCCESS)
		{
			CCT_ASSERT_FALSE("ConcertoGraphics: vkCreateSemaphore failed VKResult={}", static_cast<int>(m_lastResult));
			return std::numeric_limits<UInt32>::max();
		}
		return m_currentImageIndex;
	}

	bool SwapChain::ReCreate(VkFormat pixelFormat, VkFormat depthFormat)
	{
		CCT_GFX_AUTO_PROFILER_SCOPE();

		m_windowExtent = { m_window.GetWidth(), m_window.GetHeight() };
		if (m_handle)
			m_device->vkDestroySwapchainKHR(*m_device->Get(), m_handle, nullptr);
		if (m_surface)
			m_device->GetInstance().vkDestroySurfaceKHR(*GetDevice()->GetInstance().Get(), m_surface, nullptr);
		NativeWindow nativeWindow = m_window.GetNativeWindow();
#if defined(CCT_PLATFORM_WINDOWS)
		const VkWin32SurfaceCreateInfoKHR createInfo = {
			.sType = VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR,
			.pNext = nullptr,
			.flags = 0,
			.hinstance = static_cast<HINSTANCE>(nativeWindow.hinstance),
			.hwnd = static_cast<HWND>(nativeWindow.window)
		};
		VkResult result = m_device->GetInstance().vkCreateWin32SurfaceKHR(*m_device->GetInstance().Get(), &createInfo, nullptr, &m_surface);
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
		m_swapChainImageViews.reset();
		m_swapChainImages.reset();
		m_depthImage = GetDevice()->GetAllocator().AllocateImage(m_windowExtent, depthFormat, VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT);
		m_depthImageView = ImageView(*GetDevice(), m_depthImage, VK_IMAGE_ASPECT_DEPTH_BIT);
		PhysicalDevice::SurfaceSupportDetails surfaceSupportDetails = m_physicalDevice.GetSurfaceSupportDetails(m_surface);
		VkSwapchainCreateInfoKHR swapChainCreateInfo = {};
		UInt32 imageCount = surfaceSupportDetails.capabilities.minImageCount + 1;
		if (surfaceSupportDetails.capabilities.maxImageCount > 0 && imageCount > surfaceSupportDetails.capabilities.maxImageCount)
			imageCount = surfaceSupportDetails.capabilities.maxImageCount;

		swapChainCreateInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
		swapChainCreateInfo.surface = m_surface;
		swapChainCreateInfo.minImageCount = imageCount;
		swapChainCreateInfo.imageFormat = pixelFormat;
		swapChainCreateInfo.imageColorSpace = VK_COLOR_SPACE_SRGB_NONLINEAR_KHR;
		swapChainCreateInfo.imageExtent = m_windowExtent;
		swapChainCreateInfo.imageArrayLayers = 1;
		swapChainCreateInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
		swapChainCreateInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
		swapChainCreateInfo.preTransform = VK_SURFACE_TRANSFORM_IDENTITY_BIT_KHR;
		swapChainCreateInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
		swapChainCreateInfo.presentMode = VK_PRESENT_MODE_IMMEDIATE_KHR;
		swapChainCreateInfo.clipped = VK_TRUE;
		swapChainCreateInfo.oldSwapchain = VK_NULL_HANDLE;
		m_lastResult = m_device->vkCreateSwapchainKHR(*m_device->Get(), &swapChainCreateInfo, nullptr, &m_handle);
		if (m_lastResult != VK_SUCCESS)
		{
			CCT_ASSERT_FALSE("ConcertoGraphics: vkCreateSwapchainKHR failed VKResult={}",
								  static_cast<int>(m_lastResult));
			return false;
		}
		return true;
	}
}
