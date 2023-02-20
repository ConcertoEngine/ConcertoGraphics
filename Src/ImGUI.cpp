//
// Created by arthu on 21/12/2022.
//

#include "ImGUI.hpp"

#include "imgui_impl_glfw.h"
#include "imgui_impl_vulkan.h"

#include "Vulkan/Wrapper/Instance.hpp"
#include "Vulkan/Wrapper/PhysicalDevice.hpp"
#include "Vulkan/Wrapper/Device.hpp"
#include "Vulkan/Wrapper/RenderPass.hpp"
#include "Vulkan/Wrapper/CommandBuffer.hpp"
#include "GlfW3.hpp"

namespace Concerto::Graphics
{
	Graphics::ImGUI::ImGUI(RenderingContext& context, GlfW3& window) : _context(context),
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
		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO();
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
		init_info.Subpass = _context.subPass;
		ImGui_ImplVulkan_Init(&init_info, *_context.renderPass->Get());
		ImGui_ImplGlfw_InitForVulkan((GLFWwindow*)window.GetRawWindow(), true);
		_context.commandBuffer->ImmediateSubmit(*_context.fence, *_context.commandPool, *_context.queue,
				[](Wrapper::CommandBuffer& cb)
				{
					ImGui_ImplVulkan_CreateFontsTexture(cb.Get());
				});
	}

	Graphics::ImGUI::~ImGUI()
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

	void ImGUI::RenderDrawData(Wrapper::CommandBuffer& commandBuffer)
	{
		ImGui_ImplVulkan_RenderDrawData(ImGui::GetDrawData(), commandBuffer.Get());
	}
} // Concerto::Graphics