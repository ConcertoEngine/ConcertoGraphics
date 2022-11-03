//
// Created by arthur on 16/06/22.
//

#include <cassert>
#include <stdexcept>

#include "wrapper/DescriptorSet.hpp"
#include "wrapper/DescriptorSetLayout.hpp"
#include "wrapper/DescriptorPool.hpp"
#include "wrapper/Device.hpp"
#include "wrapper/Sampler.hpp"
#include "wrapper/ImageView.hpp"

namespace Concerto::Graphics::Wrapper
{
	DescriptorSet::DescriptorSet(Device& device, DescriptorPool& pool,
			DescriptorSetLayout& descriptorSetLayout) :  Object<VkDescriptorSet>(device)
	{
		VkDescriptorSetAllocateInfo allocInfo = {};
		allocInfo.pNext = nullptr;
		allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
		allocInfo.descriptorPool = *pool.Get();
		allocInfo.descriptorSetCount = 1;
		allocInfo.pSetLayouts = descriptorSetLayout.Get();

		if (vkAllocateDescriptorSets(*device.Get(), &allocInfo, &_handle) != VK_SUCCESS)
		{
			throw std::runtime_error("Unable to allocate descriptor sets");
		}
	}

	void DescriptorSet::WriteImageSamplerDescriptor(Sampler& sampler, ImageView& imageView, VkImageLayout imageLayout)
	{
		VkDescriptorImageInfo imageBufferInfo;
		imageBufferInfo.sampler = *sampler.Get();
		imageBufferInfo.imageView = *imageView.Get();
		imageBufferInfo.imageLayout = imageLayout;

		VkWriteDescriptorSet texture1 = VulkanInitializer::WriteDescriptorImage(
				VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, _handle, &imageBufferInfo, 0);

		vkUpdateDescriptorSets(*_device->Get(), 1, &texture1, 0, nullptr);
	}



}