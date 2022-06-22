//
// Created by arthur on 16/06/22.
//


#include "wrapper/DescriptorSet.hpp"
#include <stdexcept>
namespace Concerto::Graphics::Wrapper
{

	DescriptorSet::DescriptorSet(VkDevice device, DescriptorPool& pool,
			DescriptorSetLayout& descriptorSetLayout)
	{
		VkDescriptorSetAllocateInfo allocInfo = {};
		auto layout = descriptorSetLayout.get();
		allocInfo.pNext = nullptr;
		allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
		allocInfo.descriptorPool = pool.get();
		allocInfo.descriptorSetCount = 1;
		allocInfo.pSetLayouts = &layout;

		if (vkAllocateDescriptorSets(device, &allocInfo, &_set) != VK_SUCCESS)
		{
			throw std::runtime_error("Unable to allocate descriptor sets");
		}
	}

	VkDescriptorSet DescriptorSet::get() const
	{
		return _set;
	}
}