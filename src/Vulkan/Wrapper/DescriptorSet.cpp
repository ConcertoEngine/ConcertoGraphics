//
// Created by arthur on 16/06/22.
//

#include <cassert>
#include <stdexcept>
#include <iostream>

#include "Vulkan/wrapper/DescriptorSet.hpp"
#include "Vulkan/wrapper/DescriptorSetLayout.hpp"
#include "Vulkan/wrapper/DescriptorPool.hpp"
#include "Vulkan/wrapper/Device.hpp"
#include "Vulkan/wrapper/Sampler.hpp"
#include "Vulkan/wrapper/ImageView.hpp"

namespace Concerto::Graphics::Wrapper
{
	DescriptorSet::DescriptorSet(Device& device, DescriptorPool& pool,
			DescriptorSetLayout& descriptorSetLayout) : Object<VkDescriptorSet>(device, [this](Device &device, VkDescriptorSet handle)
	{
		if (_pool != nullptr)
			vkFreeDescriptorSets(*device.Get(), *_pool->Get(), 1, &handle);
	}), _pool(&pool)
	{
		VkDescriptorSetAllocateInfo allocInfo = {};
		allocInfo.pNext = nullptr;
		allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
		allocInfo.descriptorPool = *pool.Get();
		allocInfo.descriptorSetCount = 1;
		allocInfo.pSetLayouts = descriptorSetLayout.Get();
		_lastResult = vkAllocateDescriptorSets(*_device->Get(), &allocInfo, &_handle);
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

	DescriptorSet::DescriptorSet(DescriptorSet&& other) noexcept : Object<VkDescriptorSet>(std::move(other))
	{
		_pool = std::exchange(other._pool, nullptr);
	}
}