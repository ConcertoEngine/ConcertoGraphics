//
// Created by arthur on 12/06/22.
//


#include <stdexcept>
#include <cassert>

#include "wrapper/RenderPass.hpp"

namespace Concerto::Graphics::Wrapper
{
	RenderPass::RenderPass(VkDevice device, std::vector<VkAttachmentDescription> attachments,
			std::vector<VkSubpassDescription> subpasses, std::vector<VkSubpassDependency> dependencies) :
			_renderPass(VK_NULL_HANDLE), _device(device), _attachments(attachments), _subpasses(subpasses),
			_dependencies(dependencies)
	{
		VkRenderPassCreateInfo render_pass_info = {};
		render_pass_info.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
		render_pass_info.attachmentCount = attachments.size();
		render_pass_info.pAttachments = attachments.data();
		render_pass_info.subpassCount = subpasses.size();
		render_pass_info.pSubpasses = subpasses.data();
		render_pass_info.dependencyCount = dependencies.size();
		render_pass_info.pDependencies = dependencies.data();

		if (vkCreateRenderPass(_device, &render_pass_info, nullptr, &_renderPass) != VK_SUCCESS)
		{
			throw std::runtime_error("failed to create render pass!");
		}
	}

	RenderPass::~RenderPass()
	{
		vkDestroyRenderPass(_device, _renderPass, nullptr);
		_renderPass = VK_NULL_HANDLE;
	}

	VkRenderPass RenderPass::Get() const
	{
		assert(_renderPass != VK_NULL_HANDLE);
		return _renderPass;
	}
} // Concerto::Graphics::Wrapper
