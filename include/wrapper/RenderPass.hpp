//
// Created by arthur on 12/06/22.
//

#ifndef CONCERTOGRAPHICS_RENDERPASS_HPP
#define CONCERTOGRAPHICS_RENDERPASS_HPP

#include <vector>
#include "vulkan/vulkan.h"
#include "wrapper/Object.hpp"

namespace Concerto::Graphics::Wrapper
{
	class Swapchain;

	class Device;

	class RenderPass : public Object<VkRenderPass>
	{
	public:
		RenderPass(Device& device, Swapchain& swapchain);

		RenderPass(RenderPass&&) = default;

		RenderPass(const RenderPass&) = delete;

		RenderPass& operator=(RenderPass&&) = default;

		RenderPass& operator=(const RenderPass&) = delete;

		~RenderPass();

	private:
		std::vector<VkAttachmentDescription> _attachments;
		std::vector<VkSubpassDescription> _subpasses;
		std::vector<VkSubpassDependency> _dependencies;
	};

} // Concerto

#endif //CONCERTOGRAPHICS_RENDERPASS_HPP
