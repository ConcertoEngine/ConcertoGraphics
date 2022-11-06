//
// Created by arthur on 12/07/2022.
//
#define VMA_IMPLEMENTATION
#define VMA_VULKAN_VERSION 1000000
#define VKB_DEBUG

#include "VulkanRenderer.hpp"

int main()
{
	using namespace Concerto::Graphics;
	RendererInfo info = {
			.applicationName = "Concerto Graphics",
			.applicationVersion = { 1, 0, 0 },
			.width = 1280,
			.height = 720,
	};
	VulkanRenderer engine(std::move(info));
	while (true)
	{
		engine.DrawObject("./assets/monkey_smooth.obj", {}, 0, 0, 0, 0, 0, 0, 1, 1, 1);
		engine.Draw();
	}
}
