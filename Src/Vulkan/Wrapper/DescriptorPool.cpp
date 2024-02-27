//
// Created by arthur on 16/06/22.
//


#include <stdexcept>
#include <cassert>

#include "Concerto/Graphics/Vulkan/Wrapper/DescriptorPool.hpp"

#include <Concerto/Core/Assert.hpp>

#include "Concerto/Graphics/Vulkan/Wrapper/Device.hpp"

namespace Concerto::Graphics
{
	DescriptorPool::DescriptorPool(Device& device) : Object(device)
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
		pool_info.poolSizeCount = static_cast<UInt32>(sizes.size());
		pool_info.pPoolSizes = sizes.data();
		if (vkCreateDescriptorPool(*device.Get(), &pool_info, nullptr, &_handle) != VK_SUCCESS)
		{
			CONCERTO_ASSERT_FALSE;
			throw std::runtime_error("Failed to create descriptor pool");
		}
	}

	DescriptorPool::DescriptorPool(Device& device, std::vector<VkDescriptorPoolSize> poolSizes) : Object<VkDescriptorPool>(device)
	{
		VkDescriptorPoolCreateInfo pool_info = {};
		pool_info.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
		pool_info.flags = 0;
		pool_info.maxSets = 10;
		pool_info.poolSizeCount = static_cast<UInt32>(poolSizes.size());
		pool_info.pPoolSizes = poolSizes.data();
		if (vkCreateDescriptorPool(*device.Get(), &pool_info, nullptr, &_handle) != VK_SUCCESS)
		{
			CONCERTO_ASSERT_FALSE;
			throw std::runtime_error("Failed to create descriptor pool");
		}
	}


	DescriptorPool::~DescriptorPool()
	{
		if (IsNull())
			return;
		vkDestroyDescriptorPool(*_device->Get(), _handle, nullptr);
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