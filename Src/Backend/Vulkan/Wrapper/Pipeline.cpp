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
		m_pipelineInfo(std::move(pipeLineInfo)),
		m_createInfo()
	{
		m_createInfo.viewportState = BuildViewportState();
		m_createInfo.colorBlend = BuildColorBlendState();
		m_createInfo.pipelineCreateInfo = {};
		m_createInfo.pipelineCreateInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
	}

	Pipeline::~Pipeline()
	{
		if (IsNull())
			return;
		m_device->vkDestroyPipeline(*m_device->Get(), m_handle, nullptr);
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

		pipelineInfo.pStages = m_pipelineInfo.m_shaderStages.data();
		pipelineInfo.stageCount = static_cast<UInt32>(m_pipelineInfo.m_shaderStages.size());
		pipelineInfo.pVertexInputState = &m_pipelineInfo.m_vertexInputInfo;
		pipelineInfo.pInputAssemblyState = &m_pipelineInfo.m_inputAssembly;
		pipelineInfo.pViewportState = &viewportState;
		pipelineInfo.pDynamicState = &dynamicStateCreateInfo;
		pipelineInfo.pRasterizationState = &m_pipelineInfo.m_rasterizer;
		pipelineInfo.pMultisampleState = &m_pipelineInfo.m_multisampling;
		pipelineInfo.pColorBlendState = &colorBlending;
		pipelineInfo.layout = *m_pipelineInfo.m_pipelineLayout->Get();
		pipelineInfo.renderPass = renderPass;
		pipelineInfo.subpass = 0;
		pipelineInfo.basePipelineHandle = VK_NULL_HANDLE;
		pipelineInfo.pDepthStencilState = &m_pipelineInfo.m_depthStencil;


		if (m_device->vkCreateGraphicsPipelines(*m_device->Get(), VK_NULL_HANDLE, 1, &pipelineInfo, nullptr, &m_handle) !=
			VK_SUCCESS)
		{
			std::cerr << "failed to create pipeline\n";
			return VK_NULL_HANDLE;
		}
		return m_handle;
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
		//		viewportState.pViewports = &m_pipelineInfo.m_viewport;
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
		return m_pipelineInfo.m_pipelineLayout;
	}
}
