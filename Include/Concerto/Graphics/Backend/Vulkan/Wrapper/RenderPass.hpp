//
// Created by arthur on 12/06/22.
//

#ifndef CONCERTO_GRAPHICS_RENDERPASS_HPP
#define CONCERTO_GRAPHICS_RENDERPASS_HPP

#include <vector>
#include <span>


#include "Concerto/Graphics/Backend/Vulkan/Defines.hpp"
#include "Concerto/Graphics/Backend/Vulkan/Wrapper/Object.hpp"

namespace cct::gfx::vk
{
	class Device;

	class CONCERTO_GRAPHICS_VULKAN_BACKEND_API RenderPass : public Object<VkRenderPass>
	{
	public:
		RenderPass(Device& device, std::span<VkAttachmentDescription> attachmentDescriptions, std::span<VkSubpassDescription> subPassDescriptions, std::span<VkSubpassDependency> subPassDependencies);
		~RenderPass() override;

		RenderPass(RenderPass&&) = default;
		RenderPass(const RenderPass&) = delete;

		RenderPass& operator=(RenderPass&&) = default;
		RenderPass& operator=(const RenderPass&) = delete;

	private:
		std::vector<VkAttachmentDescription> _attachments;
		std::vector<VkSubpassDescription> _subpasses;
		std::vector<VkSubpassDependency> _dependencies;
	};

} // Concerto

#endif //CONCERTO_GRAPHICS_RENDERPASS_HPP
