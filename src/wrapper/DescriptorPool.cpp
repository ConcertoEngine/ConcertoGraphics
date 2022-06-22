//
// Created by arthur on 16/06/22.
//


#include "wrapper/DescriptorPool.hpp"
#include <stdexcept>

namespace Concerto::Graphics::Wrapper
{

	DescriptorPool::DescriptorPool(VkDevice device, std::vector<VkDescriptorPoolSize> poolSizes) : _device(device), _pool(VK_NULL_HANDLE)
	{
		VkDescriptorPoolCreateInfo pool_info = {};
		pool_info.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
		pool_info.flags = 0;
		pool_info.maxSets = 10;
		pool_info.poolSizeCount = (std::uint32_t)poolSizes.size();
		pool_info.pPoolSizes = poolSizes.data();
		if (vkCreateDescriptorPool(device, &pool_info, nullptr, &_pool) != VK_SUCCESS)
		{
			throw std::runtime_error("Failed to create descriptor pool");
		}
	}

	DescriptorPool::~DescriptorPool()
	{
		vkDestroyDescriptorPool(_device, _pool, nullptr);
	}

	VkDescriptorPool DescriptorPool::get() const
	{
		return _pool;
	}
}