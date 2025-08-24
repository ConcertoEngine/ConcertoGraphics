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

namespace cct::gfx::vk
{
	DescriptorSet::DescriptorSet(Device& device, DescriptorPool& pool, const DescriptorSetLayout& descriptorSetLayout) :
		Object(device),
		m_pool(&pool)
	{
		VkDescriptorSetAllocateInfo allocInfo = {};
		allocInfo.pNext = nullptr;
		allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
		allocInfo.descriptorPool = *pool.Get();
		allocInfo.descriptorSetCount = 1;
		allocInfo.pSetLayouts = descriptorSetLayout.Get();
		m_lastResult = m_device->vkAllocateDescriptorSets(*m_device->Get(), &allocInfo, &m_handle);
	}

	DescriptorSet::~DescriptorSet()
	{
		if (IsNull())
			return;
		if (m_pool == nullptr)
		{
			CCT_ASSERT_FALSE("ConcertoGraphics: Trying to destroy a descriptor set with an invalid pool");
			return;
		}
		m_device->vkFreeDescriptorSets(*m_device->Get(), *m_pool->Get(), 1, &m_handle);
	}

	void DescriptorSet::WriteImageSamplerDescriptor(const Sampler& sampler, const ImageView& imageView, VkImageLayout imageLayout) const
	{
		VkDescriptorImageInfo imageBufferInfo;
		imageBufferInfo.sampler = *sampler.Get();
		imageBufferInfo.imageView = *imageView.Get();
		imageBufferInfo.imageLayout = imageLayout;

		const VkWriteDescriptorSet texture1 = VulkanInitializer::WriteDescriptorImage(VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, m_handle, &imageBufferInfo, 0);

		m_device->vkUpdateDescriptorSets(*m_device->Get(), 1, &texture1, 0, nullptr);
	}

	DescriptorSet::DescriptorSet(DescriptorSet&& other) noexcept : Object<VkDescriptorSet>(std::move(other))
	{
		m_pool = std::exchange(other.m_pool, nullptr);
	}

	DescriptorSet& DescriptorSet::operator=(DescriptorSet&& other) noexcept
	{
		m_pool = std::exchange(other.m_pool, nullptr);
		return *this;
	}
}
