//
// Created by arthur on 15/05/2024.
//

#include "Concerto/Graphics/RHI/Vulkan/VkRHIRenderPass.hpp"
#include "Concerto/Graphics/RHI/Vulkan/VkRHIDevice.hpp"

namespace Concerto::Graphics::RHI
{
	VkRHIRenderPass::VkRHIRenderPass(VkRHIDevice& device, std::span<VkAttachmentDescription> attachmentDescriptions, std::span<VkSubpassDescription> subPassDescriptions, std::span<VkSubpassDependency> subPassDependencies) :
		Vk::RenderPass(device, attachmentDescriptions, subPassDescriptions, subPassDependencies)
	{
	}
}
