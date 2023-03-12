//
// Created by arthur on 16/06/22.
//


#include <stdexcept>
#include <cassert>

#include "Vulkan/wrapper/DescriptorPool.hpp"
#include "Vulkan/wrapper/Device.hpp"

namespace Concerto::Graphics::Wrapper
{
	DescriptorPool::DescriptorPool(Device& device) : Object<VkDescriptorPool>(device, [](Device &device, VkDescriptorPool handle)
	{
		vkDestroyDescriptorPool(*device.Get(), handle, nullptr);
	})
	{
		std::vector<VkDescriptorPoolSize> sizes =
				{
						{ VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,         1000 },
						{ VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC, 1000 },
						{ VK_DESCRIPTOR_TYPE_STORAGE_BUFFER,         1000 },
						{ VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 1000 }
				};
		VkDescriptorPoolCreateInfo pool_info = {};
		pool_info.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
		pool_info.flags = 0;
		pool_info.maxSets = 1000;
		pool_info.poolSizeCount = sizes.size();
		pool_info.pPoolSizes = sizes.data();
		if (vkCreateDescriptorPool(*device.Get(), &pool_info, nullptr, &_handle) != VK_SUCCESS)
		{
			throw std::runtime_error("Failed to create descriptor pool");
		}
	}

	DescriptorPool::DescriptorPool(Device& device, std::vector<VkDescriptorPoolSize> poolSizes)
			: Object<VkDescriptorPool>(device, [](Device &device, VkDescriptorPool handle)
	{
		vkDestroyDescriptorPool(*device.Get(), handle, nullptr);
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

	void DescriptorPool::Reset()
	{
		vkResetDescriptorPool(*_device->Get(), _handle, 0);
	}
}