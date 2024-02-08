//
// Created by arthur on 16/02/2023.
//

#include "Concerto/Graphics/Vulkan/DescriptorBuilder.hpp"

#include "Concerto/Graphics/Vulkan/DescriptorAllocator.hpp"
#include "Concerto/Graphics/Vulkan/DescriptorLayoutCache.hpp"
#include "Concerto/Graphics/Vulkan/Wrapper/DescriptorSet.hpp"
#include "Concerto/Graphics/Vulkan/Wrapper/DescriptorSetLayout.hpp"

namespace Concerto::Graphics
{

	DescriptorBuilder::DescriptorBuilder(DescriptorLayoutCache& layoutCache, DescriptorAllocator& allocator) :
		_cache(layoutCache), _alloc(allocator)
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

		_bindings.push_back(newBinding);

		VkWriteDescriptorSet newWrite{};
		newWrite.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
		newWrite.pNext = nullptr;

		newWrite.descriptorCount = 1;
		newWrite.descriptorType = type;
		newWrite.pBufferInfo = bufferInfo;
		newWrite.dstBinding = binding;

		_writes.push_back(newWrite);
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

		_bindings.push_back(newBinding);

		VkWriteDescriptorSet newWrite{};
		newWrite.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
		newWrite.pNext = nullptr;

		newWrite.descriptorCount = 1;
		newWrite.descriptorType = type;
		newWrite.pImageInfo = imageInfo;
		newWrite.dstBinding = binding;

		_writes.push_back(newWrite);
		return *this;
	}

	bool DescriptorBuilder::Build(std::shared_ptr<DescriptorSet>& set, std::shared_ptr<DescriptorSetLayout>& layout)
	{
		VkDescriptorSetLayoutCreateInfo layoutInfo{};
		layoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
		layoutInfo.pNext = nullptr;

		layoutInfo.pBindings = _bindings.data();
		layoutInfo.bindingCount = _bindings.size();

		layout = _cache.GetLayout(layoutInfo);

		const bool success = _alloc.Allocate(set, *layout);
		if (!success)
			return false;

		for (VkWriteDescriptorSet& w : _writes)
			w.dstSet = *set->Get();

		vkUpdateDescriptorSets(*_alloc.GetDevice().Get(), _writes.size(), _writes.data(), 0, nullptr);
		_bindings.clear();
		_writes.clear();
		return true;
	}

	bool DescriptorBuilder::Build(std::shared_ptr<DescriptorSet>& set)
	{
		DescriptorSetLayoutPtr layout;
		return Build(set, layout);
	}
}