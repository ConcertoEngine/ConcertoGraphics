//
// Created by arthur on 30/06/2022.
//

#include <stdexcept>

#include "Concerto/Graphics/Backend/Vulkan/Wrapper/DescriptorSetLayout.hpp"
#include "Concerto/Graphics/Backend/Vulkan/Wrapper/PipelineLayout.hpp"
#include "Concerto/Graphics/Backend/Vulkan/Wrapper/Device.hpp"

namespace Concerto::Graphics::Vk
{

	PipelineLayout::PipelineLayout(Device& device, std::vector<std::shared_ptr<DescriptorSetLayout>> descriptorSetLayouts) :
		Object(device),
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
		const VkResult result = vkCreatePipelineLayout(*_device->Get(), &pipelineLayoutCreateInfo, nullptr, &_handle);
		CONCERTO_ASSERT(result == VK_SUCCESS, "ConcertoGraphics: vkCreatePipelineLayout failed VKResult={}", static_cast<int>(result));
	}

	PipelineLayout::~PipelineLayout()
	{
		if (IsNull())
			return;
		vkDestroyPipelineLayout(*_device->Get(), _handle, nullptr);
	}

	const std::vector<std::shared_ptr<DescriptorSetLayout>>& PipelineLayout::GetDescriptorSetLayouts() const
	{
		return _descriptorSetLayouts;
	}
}
