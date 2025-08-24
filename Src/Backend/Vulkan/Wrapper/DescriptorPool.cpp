//
// Created by arthur on 16/06/22.
//

#include <cassert>

#include "Concerto/Graphics/Backend/Vulkan/Wrapper/DescriptorPool.hpp"

#include <Concerto/Core/Assert.hpp>

#include "Concerto/Graphics/Backend/Vulkan/Wrapper/Device.hpp"

namespace cct::gfx::vk
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
		m_lastResult = device.vkCreateDescriptorPool(*device.Get(), &descriptorPoolCreateInfo, nullptr, &m_handle);
		CCT_ASSERT(m_lastResult == VK_SUCCESS, "ConcertoGraphics: vkCreateDescriptorPool failed VkResult={}", static_cast<const int>(m_lastResult));
	}

	DescriptorPool::DescriptorPool(Device& device, std::vector<VkDescriptorPoolSize> poolSizes) : Object<VkDescriptorPool>(device)
	{
		VkDescriptorPoolCreateInfo descriptorPoolCreateInfo = {};
		descriptorPoolCreateInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
		descriptorPoolCreateInfo.maxSets = 10;
		descriptorPoolCreateInfo.poolSizeCount = static_cast<UInt32>(poolSizes.size());
		descriptorPoolCreateInfo.pPoolSizes = poolSizes.data();
		descriptorPoolCreateInfo.flags = VK_DESCRIPTOR_POOL_CREATE_FREE_DESCRIPTOR_SET_BIT;
		m_lastResult = device.vkCreateDescriptorPool(*device.Get(), &descriptorPoolCreateInfo, nullptr, &m_handle);
		CCT_ASSERT(m_lastResult == VK_SUCCESS, "ConcertoGraphics: Failed to create descriptor pool VkResult={}", static_cast<const int>(m_lastResult));
	}

	DescriptorPool::~DescriptorPool()
	{
		if (IsNull())
			return;
		m_device->vkDestroyDescriptorPool(*m_device->Get(), m_handle, nullptr);
	}

	DescriptorSet DescriptorPool::AllocateDescriptorSet(DescriptorSetLayout& setLayout)
	{
		return { *m_device, *this, setLayout };
	}

	void DescriptorPool::Reset()
	{
		m_device->vkResetDescriptorPool(*m_device->Get(), m_handle, 0);
	}
}