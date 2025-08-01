//
// Created by arthur on 10/06/22.
//

#include <iostream>
#include <utility>

#include "Concerto/Graphics/Backend/Vulkan/Wrapper/Pipeline.hpp"
#include "Concerto/Graphics/Backend/Vulkan/Wrapper/PipelineLayout.hpp"
#include "Concerto/Graphics/Backend/Vulkan/Wrapper/Device.hpp"
#include "Concerto/Graphics/Backend/Vulkan/Wrapper/VulkanInitializer.hpp"

namespace cct::gfx::vk
{

	Pipeline::Pipeline(Device& device, PipelineInfo pipeLineInfo) :
		Object(device),
		_pipelineInfo(std::move(pipeLineInfo)),
		_createInfo()
	{
		_createInfo.viewportState = BuildViewportState();
		_createInfo.colorBlend = BuildColorBlendState();
		_createInfo.pipelineCreateInfo = {};
		_createInfo.pipelineCreateInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
	}

	Pipeline::~Pipeline()
	{
		if (IsNull())
			return;
		_device->vkDestroyPipeline(*_device->Get(), _handle, nullptr);
	}

	VkPipeline Pipeline::BuildPipeline(VkRenderPass renderPass)
	{
		CCT_GFX_AUTO_PROFILER_SCOPE();

		VkGraphicsPipelineCreateInfo pipelineInfo{};
		VkPipelineColorBlendStateCreateInfo colorBlending{};
		VkPipelineViewportStateCreateInfo viewportState{};
		VkPipelineColorBlendAttachmentState colorBlendAttachment(VulkanInitializer::ColorBlendAttachmentState());

		VkDynamicState dynamicState[] = {
			VK_DYNAMIC_STATE_VIEWPORT,
			VK_DYNAMIC_STATE_SCISSOR
		};
		VkPipelineDynamicStateCreateInfo dynamicStateCreateInfo{};
		dynamicStateCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
		dynamicStateCreateInfo.dynamicStateCount = 2;
		dynamicStateCreateInfo.pDynamicStates = dynamicState;

		viewportState.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
		viewportState.pNext = nullptr;
		viewportState.viewportCount = 1;
		viewportState.scissorCount = 1;

		colorBlending.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
		colorBlending.pNext = nullptr;
		colorBlending.logicOpEnable = VK_FALSE;
		colorBlending.logicOp = VK_LOGIC_OP_COPY;
		colorBlending.attachmentCount = 1;
		colorBlending.pAttachments = &colorBlendAttachment;

		pipelineInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
		pipelineInfo.pNext = nullptr;

		pipelineInfo.pStages = _pipelineInfo._shaderStages.data();
		pipelineInfo.stageCount = static_cast<UInt32>(_pipelineInfo._shaderStages.size());
		pipelineInfo.pVertexInputState = &_pipelineInfo._vertexInputInfo;
		pipelineInfo.pInputAssemblyState = &_pipelineInfo._inputAssembly;
		pipelineInfo.pViewportState = &viewportState;
		pipelineInfo.pDynamicState = &dynamicStateCreateInfo;
		pipelineInfo.pRasterizationState = &_pipelineInfo._rasterizer;
		pipelineInfo.pMultisampleState = &_pipelineInfo._multisampling;
		pipelineInfo.pColorBlendState = &colorBlending;
		pipelineInfo.layout = *_pipelineInfo._pipelineLayout->Get();
		pipelineInfo.renderPass = renderPass;
		pipelineInfo.subpass = 0;
		pipelineInfo.basePipelineHandle = VK_NULL_HANDLE;
		pipelineInfo.pDepthStencilState = &_pipelineInfo._depthStencil;


		if (_device->vkCreateGraphicsPipelines(*_device->Get(), VK_NULL_HANDLE, 1, &pipelineInfo, nullptr, &_handle) !=
			VK_SUCCESS)
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
		//		viewportState.pViewports = &_pipelineInfo._viewport;
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

	std::shared_ptr<PipelineLayout> Pipeline::GetPipelineLayout() const
	{
		return _pipelineInfo._pipelineLayout;
	}
}
