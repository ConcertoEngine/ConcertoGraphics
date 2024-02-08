//
// Created by arthur on 30/06/2022.
//

#include <stdexcept>

#include "Concerto/Graphics/Vulkan/Wrapper/DescriptorSetLayout.hpp"
#include "Concerto/Graphics/Vulkan/Wrapper/PipelineLayout.hpp"
#include "Concerto/Graphics/Vulkan/Wrapper/Device.hpp"

namespace Concerto::Graphics
{

	PipelineLayout::PipelineLayout(Device& device, std::vector<std::shared_ptr<DescriptorSetLayout>> descriptorSetLayouts) :
		Object<VkPipelineLayout>(device, [](Device &device, VkPipelineLayout handle)
			{ vkDestroyPipelineLayout(*device.Get(), handle, nullptr); }),
		_descriptorSetLayouts(std::move(descriptorSetLayouts))
	{
		std::vector<VkDescriptorSetLayout> vkDescriptorSetLayouts;
		vkDescriptorSetLayouts.reserve(_descriptorSetLayouts.size());
		for (const auto& descriptorSetLayout : _descriptorSetLayouts)
		{
			vkDescriptorSetLayouts.push_back(*descriptorSetLayout->Get());
		}
		VkPipelineLayoutCreateInfo pipelineLayoutCreateInfo;
		pipelineLayoutCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
		pipelineLayoutCreateInfo.pNext = nullptr;
		pipelineLayoutCreateInfo.flags = 0;
		pipelineLayoutCreateInfo.setLayoutCount = vkDescriptorSetLayouts.size();
		pipelineLayoutCreateInfo.pSetLayouts = vkDescriptorSetLayouts.data();
		pipelineLayoutCreateInfo.pushConstantRangeCount = 0;
		pipelineLayoutCreateInfo.pPushConstantRanges = nullptr;
		if (vkCreatePipelineLayout(*_device->Get(), &pipelineLayoutCreateInfo, nullptr, &_handle) != VK_SUCCESS)
		{
			CONCERTO_ASSERT_FALSE;
			throw std::runtime_error("failed to create pipeline layout!");
		}
	}

	const std::vector<std::shared_ptr<DescriptorSetLayout>>& PipelineLayout::GetDescriptorSetLayouts() const
	{
		return _descriptorSetLayouts;
	}
}
