//
// Created by arthur on 12/05/2024.
//

#ifndef CONCERTO_GRAPHICS_RHI_VULKAN_VKRHIRENDERPASS_HPP
#define CONCERTO_GRAPHICS_RHI_VULKAN_VKRHIRENDERPASS_HPP

#include <span>

#include "Concerto/Graphics/Defines.hpp"
#include "Concerto/Graphics/RHI/RenderPass.hpp"
#include "Concerto/Graphics/Backend/Vulkan/Wrapper/RenderPass.hpp"

namespace Concerto::Graphics::RHI
{
	class VkRHIDevice;

	class CONCERTO_GRAPHICS_RHI_BASE_API VkRHIRenderPass final : public RHI::RenderPass, public Vk::RenderPass
	{
	public:
		VkRHIRenderPass(VkRHIDevice& device, std::span<VkAttachmentDescription> attachmentDescriptions, std::span<VkSubpassDescription> subPassDescriptions, std::span<VkSubpassDependency> subPassDependencies);
	private:
	};
}

#endif //CONCERTO_GRAPHICS_RHI_VULKAN_VKRHIRENDERPASS_HPP