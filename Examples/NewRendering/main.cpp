//
// Created by arthur on 12/07/2022.
//


#define GLFW_INCLUDE_VULKAN
#define VMA_IMPLEMENTATION
#define VMA_VULKAN_VERSION 1000000
#define VKB_DEBUG
#define GLM_FORCE_LEFT_HANDED
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <Concerto/Graphics/Vulkan.hpp>
#include <Concerto/Graphics/Vulkan/Wrapper/Device.hpp>
#include <Concerto/Graphics/Window/GlfW3.hpp>

using namespace Concerto;
using namespace Concerto::Graphics;

int main()
{
	RendererInfo info ={
		.applicationName = "Test",
		.applicationVersion = { 1, 0, 0 },
		.width = 800,
		.height = 600,
	};
	GlfW3 window("Concerto Graphics", 1280, 720);
	Vulkan vulkan(info);
	Device* device = vulkan.CreateDevice(DeviceType::Integrated);
	Swapchain swapchain(*device, window);

	while (!window.ShouldClose())
	{
	
	}
	return 0;
}
