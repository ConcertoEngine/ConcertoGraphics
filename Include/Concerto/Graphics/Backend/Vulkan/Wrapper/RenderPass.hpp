//
// Created by arthur on 12/06/22.
//

#ifndef CONCERTO_GRAPHICS_RENDERPASS_HPP
#define CONCERTO_GRAPHICS_RENDERPASS_HPP

#include <vector>
#include <span>
#include <vulkan/vulkan.h>

#include "Concerto/Graphics/Defines.hpp"
#include "Concerto/Graphics/Backend/Vulkan/Wrapper/Object.hpp"

namespace Concerto::Graphics::Vk
{
	class Device;

	/**
	 * @class RenderPass
	 * @brief A class representing a render pass.
	 *
	 * A render pass defines a set of attachments, subpasses and dependencies that describe the rendering operations that will be performed.
	 */
	class CONCERTO_GRAPHICS_API RenderPass : public Object<VkRenderPass>
	{
	public:
		/**
		 * @brief Constructs a RenderPass object.
		 * @param device The Device object associated with the render pass.
		 * @param swapchain The Swapchain object associated with the render pass.
		 */
		RenderPass(Device& device, std::span<VkAttachmentDescription> attachmentDescriptions, std::span<VkSubpassDescription> subPassDescriptions, std::span<VkSubpassDependency> subPassDependencies);

		~RenderPass();

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
