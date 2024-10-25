//
// Created by arthur on 16/06/22.
//

#include "Concerto/Graphics/Backend/Vulkan/Wrapper/DescriptorSet.hpp"
#include "Concerto/Graphics/Backend/Vulkan/Wrapper/DescriptorSetLayout.hpp"
#include "Concerto/Graphics/Backend/Vulkan/Wrapper/DescriptorPool.hpp"
#include "Concerto/Graphics/Backend/Vulkan/Wrapper/Device.hpp"
#include "Concerto/Graphics/Backend/Vulkan/Wrapper/Sampler.hpp"
#include "Concerto/Graphics/Backend/Vulkan/Wrapper/ImageView.hpp"
#include "Concerto/Graphics/Backend/Vulkan/Wrapper/VulkanInitializer.hpp"

namespace Concerto::Graphics::Vk
{
	DescriptorSet::DescriptorSet(Device& device, DescriptorPool& pool, const DescriptorSetLayout& descriptorSetLayout) :
		Object(device),
		_pool(&pool)
	{
		VkDescriptorSetAllocateInfo allocInfo = {};
		allocInfo.pNext = nullptr;
		allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
		allocInfo.descriptorPool = *pool.Get();
		allocInfo.descriptorSetCount = 1;
		allocInfo.pSetLayouts = descriptorSetLayout.Get();
		_lastResult = _device->vkAllocateDescriptorSets(*_device->Get(), &allocInfo, &_handle);
	}

	DescriptorSet::~DescriptorSet()
	{
		if (IsNull())
			return;
		if (_pool == nullptr)
		{
			CONCERTO_ASSERT_FALSE("ConcertoGraphics: Trying to destroy a descriptor set with an invalid pool");
			return;
		}
		_device->vkFreeDescriptorSets(*_device->Get(), *_pool->Get(), 1, &_handle);
	}

	void DescriptorSet::WriteImageSamplerDescriptor(const Sampler& sampler, const ImageView& imageView, VkImageLayout imageLayout) const
	{
		VkDescriptorImageInfo imageBufferInfo;
		imageBufferInfo.sampler = *sampler.Get();
		imageBufferInfo.imageView = *imageView.Get();
		imageBufferInfo.imageLayout = imageLayout;

		const VkWriteDescriptorSet texture1 = VulkanInitializer::WriteDescriptorImage(VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, _handle, &imageBufferInfo, 0);

		_device->vkUpdateDescriptorSets(*_device->Get(), 1, &texture1, 0, nullptr);
	}

	DescriptorSet::DescriptorSet(DescriptorSet&& other) noexcept : Object<VkDescriptorSet>(std::move(other))
	{
		_pool = std::exchange(other._pool, nullptr);
	}

	DescriptorSet& DescriptorSet::operator=(DescriptorSet&& other) noexcept
	{
		_pool = std::exchange(other._pool, nullptr);
		return *this;
	}
}
