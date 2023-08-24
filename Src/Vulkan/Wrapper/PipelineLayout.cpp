//
// Created by arthur on 30/06/2022.
//

#include <cassert>
#include <stdexcept>

#include "Concerto/Graphics/Vulkan/Wrapper/PipelineLayout.hpp"
#include "Concerto/Graphics/Vulkan/Wrapper/Device.hpp"

namespace Concerto::Graphics
{

	PipelineLayout::PipelineLayout(Device& device, //std::size_t size,
			const std::vector<VkDescriptorSetLayout>& descriptorSetLayouts)
			: Object<VkPipelineLayout>(device, [](Device &device, VkPipelineLayout handle)
	{ vkDestroyPipelineLayout(*device.Get(), handle, nullptr); })
	{
		VkPipelineLayoutCreateInfo pipelineLayoutCreateInfo;
		pipelineLayoutCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
		pipelineLayoutCreateInfo.pNext = nullptr;
		pipelineLayoutCreateInfo.flags = 0;
		pipelineLayoutCreateInfo.setLayoutCount = descriptorSetLayouts.size();
		pipelineLayoutCreateInfo.pSetLayouts = descriptorSetLayouts.data();
		pipelineLayoutCreateInfo.pushConstantRangeCount = 0;
		pipelineLayoutCreateInfo.pPushConstantRanges = nullptr;
		if (vkCreatePipelineLayout(*_device->Get(), &pipelineLayoutCreateInfo, nullptr, &_handle) != VK_SUCCESS)
		{
			throw std::runtime_error("failed to create pipeline layout!");
		}
	}
}
