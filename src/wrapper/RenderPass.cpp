//
// Created by arthur on 12/06/22.
//


#include <stdexcept>

#include "wrapper/RenderPass.hpp"

namespace Concerto::Graphics::Wrapper
{
	RenderPass::RenderPass(VkDevice device, std::vector<VkAttachmentDescription> attachments,
			std::vector<VkSubpassDescription> subpasses, std::vector<VkSubpassDependency> dependencies) :
			_renderPass(VK_NULL_HANDLE), _device(device), _attachments(attachments), _subpasses(subpasses),
			_dependencies(dependencies)
	{
		VkRenderPassCreateInfo render_pass_info = {
				VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO,
				nullptr,
				0,
				static_cast<std::uint32_t>(_attachments.size()),
				_attachments.data(),
				static_cast<std::uint32_t>(_subpasses.size()),
				_subpasses.data(),
				static_cast<std::uint32_t>(_dependencies.size()),
				_dependencies.data()

		};
		if (vkCreateRenderPass(_device, &render_pass_info, nullptr, &_renderPass) != VK_SUCCESS)
		{
			throw std::runtime_error("failed to create render pass!");
		}
	}

	RenderPass::~RenderPass()
	{
		vkDestroyRenderPass(_device, _renderPass, nullptr);
	}

	VkRenderPass RenderPass::getRenderPass() const
	{
		return _renderPass;
	}
} // Concerto::Graphics::Wrapper
