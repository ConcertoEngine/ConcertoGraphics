//
// Created by arthur on 30/06/2022.
//

#include <cassert>
#include <stdexcept>

#include "wrapper/PipelineLayout.hpp"
#include "wrapper/Device.hpp"

namespace Concerto::Graphics::Wrapper
{

	PipelineLayout::PipelineLayout(Device& device, std::size_t size,
			const std::vector<std::reference_wrapper<DescriptorSetLayout>>& descriptorSetLayouts)
			: Object<VkPipelineLayout>(device, [](Device &device, VkPipelineLayout handle)
	{ vkDestroyPipelineLayout(*device.Get(), handle, nullptr); })
	{
		VkPipelineLayoutCreateInfo pipelineLayoutCreateInfo;
		VkPushConstantRange push_constant;
		std::vector<VkDescriptorSetLayout> set_layout;
		set_layout.reserve(descriptorSetLayouts.size());
		for (const auto& descriptorSetLayout: descriptorSetLayouts)
		{
			set_layout.push_back(*descriptorSetLayout.get().Get());
		}
		push_constant.offset = 0;
		push_constant.size = size;
		push_constant.stageFlags = VK_SHADER_STAGE_VERTEX_BIT;
		pipelineLayoutCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
		pipelineLayoutCreateInfo.pNext = nullptr;
		pipelineLayoutCreateInfo.flags = 0;
		pipelineLayoutCreateInfo.setLayoutCount = set_layout.size();
		pipelineLayoutCreateInfo.pSetLayouts = set_layout.data();
		pipelineLayoutCreateInfo.pushConstantRangeCount = 1;
		pipelineLayoutCreateInfo.pPushConstantRanges = &push_constant;
		if (vkCreatePipelineLayout(*_device->Get(), &pipelineLayoutCreateInfo, nullptr, &_handle) != VK_SUCCESS)
		{
			throw std::runtime_error("failed to create pipeline layout!");
		}
	}
}
