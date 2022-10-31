//
// Created by arthur on 16/06/22.
//

#include <cassert>
#include <stdexcept>

#include "wrapper/DescriptorSet.hpp"
#include "wrapper/DescriptorSetLayout.hpp"
#include "wrapper/DescriptorPool.hpp"
#include "wrapper/Sampler.hpp"
#include "wrapper/ImageView.hpp"

namespace Concerto::Graphics::Wrapper
{

	DescriptorSet::DescriptorSet(VkDevice device, DescriptorPool& pool,
			DescriptorSetLayout& descriptorSetLayout) : _device(device)
	{
		VkDescriptorSetAllocateInfo allocInfo = {};
		allocInfo.pNext = nullptr;
		allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
		allocInfo.descriptorPool = *pool.Get();
		allocInfo.descriptorSetCount = 1;
		allocInfo.pSetLayouts = descriptorSetLayout.Get();

		if (vkAllocateDescriptorSets(device, &allocInfo, &_set) != VK_SUCCESS)
		{
			throw std::runtime_error("Unable to allocate descriptor sets");
		}
	}

	VkDescriptorSet* DescriptorSet::Get()
	{
		assert(_set != VK_NULL_HANDLE);
		return &_set;
	}

	bool DescriptorSet::IsValid() const
	{
		return _set != VK_NULL_HANDLE;
	}

	void DescriptorSet::WriteImageSamplerDescriptor(Sampler& sampler, ImageView& imageView, VkImageLayout imageLayout)
	{
		VkDescriptorImageInfo imageBufferInfo;
		imageBufferInfo.sampler = *sampler.Get();
		imageBufferInfo.imageView = *imageView.Get();
		imageBufferInfo.imageLayout = imageLayout;

		VkWriteDescriptorSet texture1 = VulkanInitializer::WriteDescriptorImage(
				VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, _set, &imageBufferInfo, 0);

		vkUpdateDescriptorSets(_device, 1, &texture1, 0, nullptr);
	}

}