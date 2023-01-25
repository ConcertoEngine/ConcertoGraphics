//
// Created by arthur on 16/06/22.
//

#include <cassert>
#include <stdexcept>

#include "wrapper/DescriptorSetLayout.hpp"
#include "wrapper/Device.hpp"

namespace Concerto::Graphics::Wrapper
{

	DescriptorSetLayout::DescriptorSetLayout(Device& device, std::vector<VkDescriptorSetLayoutBinding> bindings)
			: Object<VkDescriptorSetLayout>(device, [](Device &device, VkDescriptorSetLayout handle)
	{
		vkDestroyDescriptorSetLayout(*device.Get(), handle, nullptr);
	})
	{
		VkDescriptorSetLayoutCreateInfo createInfo{};
		createInfo.flags = 0;
		createInfo.pNext = nullptr;
		createInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
		createInfo.pBindings = bindings.data();
		createInfo.bindingCount = bindings.size();
		if (vkCreateDescriptorSetLayout(*_device->Get(), &createInfo, nullptr, &_handle) != VK_SUCCESS)
		{
			throw std::runtime_error("failed to create descriptor set layout!");
		}
	}
}
