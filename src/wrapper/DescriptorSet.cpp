//
// Created by arthur on 16/06/22.
//


#include "wrapper/DescriptorSet.hpp"
#include <stdexcept>
namespace Concerto::Graphics::Wrapper
{

	DescriptorSet::DescriptorSet(VkDevice device, DescriptorPool& pool,
			DescriptorSetLayout& descriptorSetLayout) : _device(device)
	{
		VkDescriptorSetAllocateInfo allocInfo = {};
		auto layout = descriptorSetLayout.Get();
		allocInfo.pNext = nullptr;
		allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
		allocInfo.descriptorPool = pool.Get();
		allocInfo.descriptorSetCount = 1;
		allocInfo.pSetLayouts = &layout;

		if (vkAllocateDescriptorSets(device, &allocInfo, &_set) != VK_SUCCESS)
		{
			throw std::runtime_error("Unable to allocate descriptor sets");
		}
	}

	VkDescriptorSet DescriptorSet::Get() const
	{
		return _set;
	}

}