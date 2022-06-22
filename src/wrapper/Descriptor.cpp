//
// Created by arthur on 15/06/22.
//


#include "wrapper/Descriptor.hpp"
#include <cstdint>
#include <stdexcept>

namespace Concerto::Graphics::Wrapper
{

	Descriptor::Descriptor(VkDevice device, std::vector<VkDescriptorPoolSize> poolSizes, std::vector<VkDescriptorSetLayoutBinding> bindings)
	{
		VkDescriptorPoolCreateInfo pool_info = {};
		pool_info.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
		pool_info.flags = 0;
		pool_info.maxSets = 10;
		pool_info.poolSizeCount = (std::uint32_t)poolSizes.size();
		pool_info.pPoolSizes = poolSizes.data();
		if (vkCreateDescriptorPool(device, &pool_info, nullptr, &_pool) != VK_SUCCESS)
		{
			throw std::runtime_error("failed to create descriptor pool!");
		}
	}

	VkDescriptorPool Descriptor::get() const
	{
		return _pool;
	}
}