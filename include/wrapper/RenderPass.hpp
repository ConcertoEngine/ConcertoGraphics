//
// Created by arthur on 12/06/22.
//

#ifndef CONCERTOGRAPHICS_RENDERPASS_HPP
#define CONCERTOGRAPHICS_RENDERPASS_HPP

#include <vector>
#include "vulkan/vulkan.h"
namespace Concerto::Graphics::Wrapper
{

	class RenderPass
	{
	public:
		RenderPass(VkDevice device, std::vector<VkAttachmentDescription> attachments, std::vector<VkSubpassDescription> subpasses, std::vector<VkSubpassDependency> dependencies);

		RenderPass(RenderPass&&) = default;

		RenderPass(const RenderPass&) = delete;

		RenderPass& operator=(RenderPass&&) = default;

		RenderPass& operator=(const RenderPass&) = delete;

		~RenderPass();
		[[nodiscard]] VkRenderPass getRenderPass() const;

	private:
		VkRenderPass _renderPass;
		VkDevice _device;
		std::vector<VkAttachmentDescription> _attachments;
		std::vector<VkSubpassDescription> _subpasses;
		std::vector<VkSubpassDependency> _dependencies;
	};

} // Concerto

#endif //CONCERTOGRAPHICS_RENDERPASS_HPP
