//
// Created by arthur on 15/06/22.
//


#include <cassert>
#include <stdexcept>

#include "wrapper/Descriptor.hpp"
#include "wrapper/Device.hpp"

namespace Concerto::Graphics::Wrapper
{

	Descriptor::Descriptor(Device& device, std::vector<VkDescriptorPoolSize> poolSizes,
			const std::vector<VkDescriptorSetLayoutBinding>& bindings) : Object<VkDescriptorPool>(device)
	{
		VkDescriptorPoolCreateInfo pool_info = {};
		pool_info.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
		pool_info.flags = 0;
		pool_info.maxSets = 10;
		pool_info.poolSizeCount = (std::uint32_t)poolSizes.size();
		pool_info.pPoolSizes = poolSizes.data();
		if (vkCreateDescriptorPool(*device.Get(), &pool_info, nullptr, &_handle) != VK_SUCCESS)
		{
			throw std::runtime_error("failed to create descriptor pool!");
		}
	}
}