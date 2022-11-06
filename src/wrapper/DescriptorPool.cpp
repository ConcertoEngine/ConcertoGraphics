//
// Created by arthur on 16/06/22.
//


#include <stdexcept>
#include <cassert>

#include "wrapper/DescriptorPool.hpp"
#include "wrapper/Device.hpp"

namespace Concerto::Graphics::Wrapper
{
	DescriptorPool::DescriptorPool(Device& device) : Object<VkDescriptorPool>(device, [this]()
	{
		vkDestroyDescriptorPool(*_device->Get(), _handle, nullptr);
	})
	{
		std::vector<VkDescriptorPoolSize> sizes =
				{
						{ VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,         10 },
						{ VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC, 10 },
						{ VK_DESCRIPTOR_TYPE_STORAGE_BUFFER,         10 },
						{ VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 10 }
				};
		VkDescriptorPoolCreateInfo pool_info = {};
		pool_info.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
		pool_info.flags = 0;
		pool_info.maxSets = 10;
		pool_info.poolSizeCount = sizes.size();
		pool_info.pPoolSizes = sizes.data();
		if (vkCreateDescriptorPool(*device.Get(), &pool_info, nullptr, &_handle) != VK_SUCCESS)
		{
			throw std::runtime_error("Failed to create descriptor pool");
		}
	}

	DescriptorPool::DescriptorPool(Device& device, std::vector<VkDescriptorPoolSize> poolSizes)
			: Object<VkDescriptorPool>(device, [this]()
	{
		vkDestroyDescriptorPool(*_device->Get(), _handle, nullptr);
	})
	{
		VkDescriptorPoolCreateInfo pool_info = {};
		pool_info.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
		pool_info.flags = 0;
		pool_info.maxSets = 10;
		pool_info.poolSizeCount = poolSizes.size();
		pool_info.pPoolSizes = poolSizes.data();
		if (vkCreateDescriptorPool(*device.Get(), &pool_info, nullptr, &_handle) != VK_SUCCESS)
		{
			throw std::runtime_error("Failed to create descriptor pool");
		}
	}


	DescriptorSet DescriptorPool::AllocateDescriptorSet(DescriptorSetLayout& setLayout)
	{
		return { *_device, *this, setLayout };
	}
}