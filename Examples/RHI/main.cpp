//
// Created by arthur on 12/07/2022.
//


#include <Concerto/Graphics/Window/Window.hpp>
#include <Concerto/Graphics/RHI/APIImpl.hpp>
#include <Concerto/Graphics/RHI/Instance.hpp>
#include <Concerto/Graphics/RHI/SwapChain.hpp>
#include <Concerto/Graphics/RHI/RenderPass.hpp>
#include <Concerto/Graphics/RHI/FrameBuffer.hpp>
#include <Concerto/Graphics/RHI/Texture.hpp>
#include <Concerto/Graphics/RHI/MaterialBuilder.hpp>
#include <Concerto/Graphics/RHI/CommandBuffer.hpp>

#include <Concerto/Graphics/RHI/Frame.hpp>
#include <Concerto/Graphics/RHI/Mesh.hpp>
#include <Concerto/Graphics/RHI/Vulkan/VkRHIMesh.hpp>

using namespace Concerto;
using namespace Concerto::Graphics;


int main()
{
	try
	{
		Window window("Concerto Graphics", 1280, 720);
		Input& inputManager = window.GetInputManager();

		RHI::Instance rInstance;
		std::unique_ptr<RHI::Device> device;

		std::size_t deviceIndex = 0;
		for (const auto& [name, vendor, type] : rInstance.EnumerateDevices())
		{
			if (type == RHI::DeviceType::Dedicated)
			{
				device = rInstance.CreateDevice(deviceIndex);
				break;
			}
			++deviceIndex;
		}
		std::unique_ptr<RHI::SwapChain> swapChain = device->CreateSwapChain(window);
		RHI::RenderPass& renderPass = swapChain->GetRenderPass();

		std::unique_ptr<RHI::MaterialBuilder> materialBuilder = device->CreateMaterialBuilder(swapChain->GetExtent());
		std::unique_ptr<RHI::TextureBuilder> textureBuilder = device->CreateTextureBuilder();
		RHI::VkRHIMesh mesh("./assets/sponza/sponza.obj"); //fixme
		std::shared_ptr<RHI::GpuMesh> gpuMesh = mesh.BuildGpuMesh(*materialBuilder, renderPass, *device);
		std::unique_ptr<RHI::CommandPool> commandPool = device->CreateCommandPool(RHI::QueueFamily::Graphics);


		while (!window.ShouldClose())
		{
			RHI::Frame& currentFrame = swapChain->AcquireFrame();
			//do stuff
			currentFrame.Present();
		}
	}
	catch (const std::exception& e)
	{
		Logger::Error("An unhandled exception was thrown: '{}'", e.what());
	}
	return 0;
}
