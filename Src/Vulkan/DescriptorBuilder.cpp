//
// Created by arthur on 16/02/2023.
//

#include "Concerto/Graphics/Vulkan/DescriptorBuilder.hpp"
#include "Concerto/Graphics/Vulkan/Wrapper/DescriptorSet.hpp"
#include "Concerto/Graphics/Vulkan/Wrapper/DescriptorSetLayout.hpp"

namespace Concerto::Graphics
{

	DescriptorBuilder::DescriptorBuilder(DescriptorLayoutCache& layoutCache, DescriptorAllocator& allocator) :
		cache(layoutCache), alloc(allocator)
	{

	}

	DescriptorBuilder& DescriptorBuilder::BindBuffer(UInt32 binding,
		VkDescriptorBufferInfo* bufferInfo,
		VkDescriptorType type,
		VkShaderStageFlags stageFlags)
	{
		VkDescriptorSetLayoutBinding newBinding{};

		newBinding.descriptorCount = 1;
		newBinding.descriptorType = type;
		newBinding.pImmutableSamplers = nullptr;
		newBinding.stageFlags = stageFlags;
		newBinding.binding = binding;

		bindings.push_back(newBinding);

		VkWriteDescriptorSet newWrite{};
		newWrite.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
		newWrite.pNext = nullptr;

		newWrite.descriptorCount = 1;
		newWrite.descriptorType = type;
		newWrite.pBufferInfo = bufferInfo;
		newWrite.dstBinding = binding;

		writes.push_back(newWrite);
		return *this;
	}

	DescriptorBuilder& DescriptorBuilder::BindImage(UInt32 binding,
		VkDescriptorImageInfo* imageInfo,
		VkDescriptorType type,
		VkShaderStageFlags stageFlags)
	{
		VkDescriptorSetLayoutBinding newBinding{};

		newBinding.descriptorCount = 1;
		newBinding.descriptorType = type;
		newBinding.pImmutableSamplers = nullptr;
		newBinding.stageFlags = stageFlags;
		newBinding.binding = binding;

		bindings.push_back(newBinding);

		VkWriteDescriptorSet newWrite{};
		newWrite.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
		newWrite.pNext = nullptr;

		newWrite.descriptorCount = 1;
		newWrite.descriptorType = type;
		newWrite.pImageInfo = imageInfo;
		newWrite.dstBinding = binding;

		writes.push_back(newWrite);
		return *this;
	}

	bool DescriptorBuilder::Build(DescriptorSetPtr& set, DescriptorSetLayoutPtr& layout)
	{
		VkDescriptorSetLayoutCreateInfo layoutInfo{};
		layoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
		layoutInfo.pNext = nullptr;

		layoutInfo.pBindings = bindings.data();
		layoutInfo.bindingCount = bindings.size();

		layout = cache.GetLayout(layoutInfo);

		bool success = alloc.Allocate(set, *layout);
		if (!success)
			return false;

		for (VkWriteDescriptorSet& w : writes)
			w.dstSet = *set->Get();

		vkUpdateDescriptorSets(*alloc.GetDevice().Get(), writes.size(), writes.data(), 0, nullptr);

		return true;
	}

	bool DescriptorBuilder::Build(DescriptorSetPtr& set)
	{
		DescriptorSetLayoutPtr layout;
		return Build(set, layout);
	}
}