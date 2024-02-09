//
// Created by arthur on 16/06/22.
//

#include <cassert>
#include <stdexcept>

#include "Concerto/Graphics/Vulkan/Wrapper/DescriptorSetLayout.hpp"
#include "Concerto/Graphics/Vulkan/Wrapper/Device.hpp"

namespace Concerto::Graphics
{

	DescriptorSetLayout::DescriptorSetLayout(Device& device, std::vector<VkDescriptorSetLayoutBinding> bindings)
			: Object<VkDescriptorSetLayout>(device, [](Device &device, VkDescriptorSetLayout handle)
			{
				vkDestroyDescriptorSetLayout(*device.Get(), handle, nullptr);
			}),
			_bindings(std::move(bindings))
	{
		VkDescriptorSetLayoutCreateInfo createInfo = {};
		createInfo.flags = 0;
		createInfo.pNext = nullptr;
		createInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
		createInfo.pBindings = _bindings.data();
		createInfo.bindingCount = static_cast<UInt32>(_bindings.size());
		if (vkCreateDescriptorSetLayout(*_device->Get(), &createInfo, nullptr, &_handle) != VK_SUCCESS)
		{
			CONCERTO_ASSERT_FALSE;
			throw std::runtime_error("failed to create descriptor set layout!");
		}
	}

	const std::vector<VkDescriptorSetLayoutBinding>& DescriptorSetLayout::GetBindings() const
	{
		return _bindings;
	}

	UInt64 DescriptorSetLayout::GetHash() const
	{
		return GetHash(_bindings);
	}

	UInt64 DescriptorSetLayout::GetHash(const std::vector<VkDescriptorSetLayoutBinding>& bindings)
	{
		std::hash<UInt32> hasher;
		UInt64 hash = 0;

		for (const auto& binding : bindings) {
			hash ^= hasher(binding.binding) + 0x9e3779b9 + (hash << 6) + (hash >> 2);
			hash ^= hasher(binding.descriptorType) + 0x9e3779b9 + (hash << 6) + (hash >> 2);
			hash ^= hasher(binding.descriptorCount) + 0x9e3779b9 + (hash << 6) + (hash >> 2);
			hash ^= hasher(binding.stageFlags) + 0x9e3779b9 + (hash << 6) + (hash >> 2);
		}
		return hash;
	}

	DescriptorSetLayoutPtr MakeDescriptorSetLayout(Device& device, const std::vector<VkDescriptorSetLayoutBinding>& bindings)
	{
		return std::make_shared<DescriptorSetLayout>(device, bindings);
	}
}
