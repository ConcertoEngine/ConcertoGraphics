//
// Created by arthur on 16/06/22.
//

#include <cassert>

#include "Concerto/Graphics/Backend/Vulkan/Wrapper/DescriptorPool.hpp"

#include <Concerto/Core/Assert.hpp>

#include "Concerto/Graphics/Backend/Vulkan/Wrapper/Device.hpp"

namespace Concerto::Graphics::Vk
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
		VkDescriptorPoolCreateInfo descriptorPoolCreateInfo = {};
		descriptorPoolCreateInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
		descriptorPoolCreateInfo.maxSets = 1000;
		descriptorPoolCreateInfo.poolSizeCount = static_cast<UInt32>(sizes.size());
		descriptorPoolCreateInfo.pPoolSizes = sizes.data();
		descriptorPoolCreateInfo.flags = VK_DESCRIPTOR_POOL_CREATE_FREE_DESCRIPTOR_SET_BIT;
		_lastResult = device.vkCreateDescriptorPool(*device.Get(), &descriptorPoolCreateInfo, nullptr, &_handle);
		CONCERTO_ASSERT(_lastResult == VK_SUCCESS, "ConcertoGraphics: vkCreateDescriptorPool failed VkResult={}", static_cast<const int>(_lastResult));
	}

	DescriptorPool::DescriptorPool(Device& device, std::vector<VkDescriptorPoolSize> poolSizes) : Object<VkDescriptorPool>(device)
	{
		VkDescriptorPoolCreateInfo descriptorPoolCreateInfo = {};
		descriptorPoolCreateInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
		descriptorPoolCreateInfo.maxSets = 10;
		descriptorPoolCreateInfo.poolSizeCount = static_cast<UInt32>(poolSizes.size());
		descriptorPoolCreateInfo.pPoolSizes = poolSizes.data();
		descriptorPoolCreateInfo.flags = VK_DESCRIPTOR_POOL_CREATE_FREE_DESCRIPTOR_SET_BIT;
		_lastResult = device.vkCreateDescriptorPool(*device.Get(), &descriptorPoolCreateInfo, nullptr, &_handle);
		CONCERTO_ASSERT(_lastResult == VK_SUCCESS, "ConcertoGraphics: Failed to create descriptor pool VkResult={}", static_cast<const int>(_lastResult));
	}

	DescriptorPool::~DescriptorPool()
	{
		if (IsNull())
			return;
		_device->vkDestroyDescriptorPool(*_device->Get(), _handle, nullptr);
	}

	DescriptorSet DescriptorPool::AllocateDescriptorSet(DescriptorSetLayout& setLayout)
	{
		return { *_device, *this, setLayout };
	}

	void DescriptorPool::Reset()
	{
		_device->vkResetDescriptorPool(*_device->Get(), _handle, 0);
	}
}