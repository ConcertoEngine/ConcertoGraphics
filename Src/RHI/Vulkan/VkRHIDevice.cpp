//
// Created by arthur on 15/05/2024.
//

#include "Concerto/Graphics/RHI/Vulkan/VkRHIDevice.hpp"

#include "PhysicalDevice.hpp"
#include "Concerto/Graphics/RHI/SwapChain.hpp"
#include "Concerto/Graphics/Window/Window.hpp"
#include "Concerto/Graphics/RHI/Vulkan/VkRHISwapChain.hpp"

namespace Concerto::Graphics::RHI
{
	VkRHIDevice::VkRHIDevice(Vk::PhysicalDevice& physicalDevice, Vk::Instance& instance) :
		Vk::Device(physicalDevice, instance),
		_vkInstance(&instance)
	{
	}

	std::unique_ptr<SwapChain> VkRHIDevice::CreateSwapChain(Window& window)
	{
		CONCERTO_ASSERT(_vkInstance, "ConcertoGraphics: Invalid Vulkan instance");
		if (!window.CreateVulkanSurface(*_vkInstance, &_surface))
		{
			CONCERTO_ASSERT_FALSE("ConcertoGraphics: Vulkan surface creation failed");
			return nullptr;
		}
		auto swapChain = std::make_unique<VkRHISwapChain>(*this, window);
		if (swapChain->GetLastResult() != VK_SUCCESS)
		{
			CONCERTO_ASSERT_FALSE("ConcertoGraphics: Error occured during swapchain creation error={}", static_cast<Int32>(swapChain->GetLastResult()));
			return nullptr;
		}
		GetPhysicalDevice().SetSurface(_surface);
		return swapChain;
	}
}
