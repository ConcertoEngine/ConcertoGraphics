//
// Created by arthur on 12/06/22.
//


#include <stdexcept>
#include <cassert>

#include <Concerto/Core/Assert.hpp>

#include "Concerto/Graphics/Backend/Vulkan/Wrapper/RenderPass.hpp"
#include "Concerto/Graphics/Backend/Vulkan/Wrapper/Device.hpp"

namespace cct::gfx::vk
{
	RenderPass::RenderPass(Device& device, std::span<VkAttachmentDescription> attachmentDescriptions, std::span<VkSubpassDescription> subPassDescriptions, std::span<VkSubpassDependency> subPassDependencies) :
		Object(device)
	{
		VkRenderPassCreateInfo renderPassCreateInfo = {};
		renderPassCreateInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
		renderPassCreateInfo.attachmentCount = static_cast<UInt32>(attachmentDescriptions.size());
		renderPassCreateInfo.pAttachments = attachmentDescriptions.data();
		renderPassCreateInfo.subpassCount = static_cast<UInt32>(subPassDescriptions.size());
		renderPassCreateInfo.pSubpasses = subPassDescriptions.data();
		renderPassCreateInfo.dependencyCount = static_cast<UInt32>(subPassDependencies.size());
		renderPassCreateInfo.pDependencies = subPassDependencies.data();

		const VkResult result = m_device->vkCreateRenderPass(*m_device->Get(), &renderPassCreateInfo, nullptr, &m_handle);
		CCT_ASSERT(result == VK_SUCCESS, "ConcertoGraphics: vkCreateRenderPass failed VKResult={}", static_cast<int>(result));
	}

	RenderPass::~RenderPass()
	{
		if (IsNull())
			return;
		m_device->vkDestroyRenderPass(*m_device->Get(), m_handle, nullptr);
	}
} // cct::gfx::vk
