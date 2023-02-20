//
// Created by arthur on 12/06/22.
//

#ifndef CONCERTOGRAPHICS_RENDERPASS_HPP
#define CONCERTOGRAPHICS_RENDERPASS_HPP

#include <vector>
#include "vulkan/vulkan.h"
#include "Object.hpp"

namespace Concerto::Graphics::Wrapper
{
	class Swapchain;

	class Device;

	/**
	 * @class RenderPass
	 * @brief A class representing a render pass.
	 *
	 * A render pass defines a set of attachments, subpasses and dependencies that describe the rendering operations that will be performed.
	 */
	class RenderPass : public Object<VkRenderPass>
	{
	public:
		/**
		 * @brief Constructs a RenderPass object.
		 * @param device The Device object associated with the render pass.
		 * @param swapchain The Swapchain object associated with the render pass.
		 */
		RenderPass(Device& device, Swapchain& swapchain);

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

#endif //CONCERTOGRAPHICS_RENDERPASS_HPP
