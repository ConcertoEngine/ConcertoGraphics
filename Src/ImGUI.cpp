//
// Created by arthu on 21/12/2022.
//

#include "Concerto/Graphics/ImGUI.hpp"

#include <imgui_impl_glfw.h>
#include <imgui_impl_vulkan.h>

#include "Concerto/Graphics/Backend/Vulkan/Wrapper/Instance.hpp"
#include "Concerto/Graphics/Backend/Vulkan/Wrapper/PhysicalDevice.hpp"
#include "Concerto/Graphics/Backend/Vulkan/Wrapper/Device.hpp"
#include "Concerto/Graphics/Backend/Vulkan/Wrapper/RenderPass.hpp"
#include "Concerto/Graphics/Backend/Vulkan/Wrapper/CommandBuffer.hpp"
#include "Concerto/Graphics/Window/Window.hpp"

namespace Concerto::Graphics::Vk
{
	ImGUI::ImGUI(RenderingContext& context, Window& window) : _context(context),
																	   _descriptorPool(*_context.device,
																			   {{ VK_DESCRIPTOR_TYPE_SAMPLER,                1000 },
																				{ VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 1000 },
																				{ VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE,          1000 },
																				{ VK_DESCRIPTOR_TYPE_STORAGE_IMAGE,          1000 },
																				{ VK_DESCRIPTOR_TYPE_UNIFORM_TEXEL_BUFFER,   1000 },
																				{ VK_DESCRIPTOR_TYPE_STORAGE_TEXEL_BUFFER,   1000 },
																				{ VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,         1000 },
																				{ VK_DESCRIPTOR_TYPE_STORAGE_BUFFER,         1000 },
																				{ VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC, 1000 },
																				{ VK_DESCRIPTOR_TYPE_STORAGE_BUFFER_DYNAMIC, 1000 },
																				{ VK_DESCRIPTOR_TYPE_INPUT_ATTACHMENT,       1000 }}),
																	   _window(window)
	{

		IMGUI_CHECKVERSION();
		_imGuiContext = ImGui::CreateContext();
		ImGui::StyleColorsDark();
		ImGui_ImplVulkan_InitInfo init_info = {};
		init_info.Instance = *_context.instance->Get();
		init_info.PhysicalDevice = *_context.physicalDevice->Get();
		init_info.Device = *_context.device->Get();
		init_info.QueueFamily = _context.queueFamilyIndex;
		init_info.Queue = *_context.queue->Get();
		init_info.DescriptorPool = *_descriptorPool.Get();
		init_info.MinImageCount = _context.minImageCount;
		init_info.ImageCount = _context.imageCount;
		init_info.MSAASamples = _context.MSAASamples;
		init_info.RenderPass = *_context.renderPass->Get();

		ImGui_ImplVulkan_Init(&init_info);
		const bool res = ImGui_ImplGlfw_InitForVulkan((GLFWwindow*)window.GetRawWindow(), true);
		if (res == false)
		{
			CONCERTO_ASSERT_FALSE("ConcertoGraphics: error ImGui_ImplGlfw_InitForVulkan");
			return;
		}
		_context.commandBuffer->ImmediateSubmit(*_context.fence, *_context.commandPool, *_context.queue,
				[](CommandBuffer&)
				{
					const bool res = ImGui_ImplVulkan_CreateFontsTexture();
					if (res == false)
					{
						CONCERTO_ASSERT_FALSE("ConcertoGraphics: error ImGui_ImplVulkan_CreateFontsTexture");
					}
				});
	}

	ImGUI::~ImGUI()
	{
		ImGui_ImplVulkan_Shutdown();
		ImGui_ImplGlfw_Shutdown();
		ImGui::DestroyContext();
	}

	void ImGUI::NewFrame()
	{
		ImGui_ImplVulkan_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();
	}

	void ImGUI::Draw()
	{
		ImGui::Render();
	}

	void ImGUI::RenderDrawData(CommandBuffer& commandBuffer)
	{
		ImGui_ImplVulkan_RenderDrawData(ImGui::GetDrawData(), commandBuffer.Get());
	}

	ImGuiContext* ImGUI::GetContext() const
	{
		return _imGuiContext;
	}

	void ImGUI::UpdateMousePosition(float x, float y)
	{
		ImGuiIO& io = ImGui::GetIO();
		io.AddMousePosEvent(x, y);
	}

	void ImGUI::UpdateMouseButton(int button, bool pressed)
	{
		ImGuiIO& io = ImGui::GetIO();
		io.AddMouseButtonEvent(button, pressed);
	}
} // Concerto::Graphics