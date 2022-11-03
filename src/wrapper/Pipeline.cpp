//
// Created by arthur on 10/06/22.
//

#include <cassert>
#include <iostream>
#include <utility>

#include "wrapper/Pipeline.hpp"
#include "wrapper/Device.hpp"
#include "wrapper/VulkanInitializer.hpp"

namespace Concerto::Graphics::Wrapper
{

	Pipeline::Pipeline(Device& device, PipelineInfo  pipeLineInfo) : Object<VkPipeline>(device),
																			 _pipelineInfo(std::move(pipeLineInfo)),
																			 _createInfo()
	{
		_createInfo.viewportState = BuildViewportState();
		_createInfo.colorBlend = BuildColorBlendState();
		_createInfo.pipelineCreateInfo = {};
		_createInfo.pipelineCreateInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
	}

	VkPipeline Pipeline::BuildPipeline(VkRenderPass renderPass)
	{
		VkGraphicsPipelineCreateInfo pipelineInfo{};
		VkPipelineColorBlendStateCreateInfo colorBlending{};
		VkPipelineViewportStateCreateInfo viewportState{};
		VkPipelineColorBlendAttachmentState colorBlendAttachment(VulkanInitializer::ColorBlendAttachmentState());

		viewportState.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
		viewportState.pNext = nullptr;
		viewportState.viewportCount = 1;
		viewportState.pViewports = &_pipelineInfo._viewport;
		viewportState.scissorCount = 1;
		viewportState.pScissors = &_pipelineInfo._scissor;

		colorBlending.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
		colorBlending.pNext = nullptr;
		colorBlending.logicOpEnable = VK_FALSE;
		colorBlending.logicOp = VK_LOGIC_OP_COPY;
		colorBlending.attachmentCount = 1;
		colorBlending.pAttachments = &colorBlendAttachment;

		pipelineInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
		pipelineInfo.pNext = nullptr;

		pipelineInfo.stageCount = _pipelineInfo._shaderStages.size();
		pipelineInfo.pStages = _pipelineInfo._shaderStages.data();
		pipelineInfo.pVertexInputState = &_pipelineInfo._vertexInputInfo;
		pipelineInfo.pInputAssemblyState = &_pipelineInfo._inputAssembly;
		pipelineInfo.pViewportState = &viewportState;
		pipelineInfo.pRasterizationState = &_pipelineInfo._rasterizer;
		pipelineInfo.pMultisampleState = &_pipelineInfo._multisampling;
		pipelineInfo.pColorBlendState = &colorBlending;
		pipelineInfo.layout = _pipelineInfo._pipelineLayout;
		pipelineInfo.renderPass = renderPass;
		pipelineInfo.subpass = 0;
		pipelineInfo.basePipelineHandle = VK_NULL_HANDLE;
		pipelineInfo.pDepthStencilState = &_pipelineInfo._depthStencil;


		if (vkCreateGraphicsPipelines(*_device->Get(), VK_NULL_HANDLE, 1, &pipelineInfo, nullptr, &_handle) != VK_SUCCESS)
		{
			std::cerr << "failed to create pipeline\n";
			return VK_NULL_HANDLE;
		}
		return _handle;
	}

	VkPipelineViewportStateCreateInfo Pipeline::BuildViewportState() const
	{
		VkPipelineViewportStateCreateInfo viewportState{};
		static VkRect2D scissor = { 0, 0 }; //TODO: remove static
		scissor.offset = { 0, 0 };
		scissor.extent = { 1280, 720 };
		viewportState.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
		viewportState.pNext = nullptr;
		viewportState.viewportCount = 1;
		viewportState.pViewports = &_pipelineInfo._viewport;
		viewportState.scissorCount = 1;
		viewportState.pScissors = &scissor;
		return viewportState;
	}

	VkPipelineColorBlendStateCreateInfo Pipeline::BuildColorBlendState() const
	{
		VkPipelineColorBlendStateCreateInfo colorBlending = {};
		static VkPipelineColorBlendAttachmentState colorBlendAttachment(
				VulkanInitializer::ColorBlendAttachmentState()); //TODO: remove static
		colorBlending.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
		colorBlending.pNext = nullptr;
		colorBlending.logicOpEnable = VK_FALSE;
		colorBlending.logicOp = VK_LOGIC_OP_COPY;
		colorBlending.attachmentCount = 1;
		colorBlending.pAttachments = &colorBlendAttachment;
		return colorBlending;
	}

	Pipeline::~Pipeline()
	{
		assert(_device != VK_NULL_HANDLE);
		assert(!IsNull());
		vkDestroyPipeline(*_device->Get(), _handle, nullptr);
	}
}