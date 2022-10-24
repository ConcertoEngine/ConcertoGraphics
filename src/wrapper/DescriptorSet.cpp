//
// Created by arthur on 16/06/22.
//


#include <stdexcept>
#include "wrapper/DescriptorSet.hpp"
#include "wrapper/DescriptorPool.hpp"

namespace Concerto::Graphics::Wrapper
{

	DescriptorSet::DescriptorSet(VkDevice device, DescriptorPool& pool,
			DescriptorSetLayout& descriptorSetLayout) : _device(device)
	{
		VkDescriptorSetAllocateInfo allocInfo = {};
		allocInfo.pNext = nullptr;
		allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
		allocInfo.descriptorPool = *pool.Get();
		allocInfo.descriptorSetCount = 1;
		allocInfo.pSetLayouts = descriptorSetLayout.Get();

		if (vkAllocateDescriptorSets(device, &allocInfo, &_set) != VK_SUCCESS)
		{
			throw std::runtime_error("Unable to allocate descriptor sets");
		}
	}

	VkDescriptorSet* DescriptorSet::Get()
	{
		return &_set;
	}

	bool DescriptorSet::IsValid() const
	{
		return _set != VK_NULL_HANDLE;
	}

}