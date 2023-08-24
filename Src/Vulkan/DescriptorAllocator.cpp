//
// Created by arthur on 16/02/2023.
//

#define DESCRIPTOR_POOL_SIZE 1000

#include "Concerto/Graphics/Vulkan/DescriptorAllocator.hpp"
#include "Concerto/Graphics/Vulkan/Wrapper/DescriptorSet.hpp"
#include "Concerto/Graphics/Vulkan/Wrapper/DescriptorSetLayout.hpp"
#include "Concerto/Graphics/Vulkan/Wrapper/Device.hpp"

namespace Concerto::Graphics
{

	DescriptorAllocator::DescriptorAllocator(Device& device) :
		_device(&device),
		_currentPool(VK_NULL_HANDLE)
	{

	}

	bool DescriptorAllocator::Allocate(DescriptorSetPtr& descriptorSet, DescriptorSetLayout& layout)
	{
		if (_currentPool == VK_NULL_HANDLE)
		{
			_currentPool = GetPool();
			_usedPools.push_back(_currentPool);
		}
		descriptorSet = std::make_shared<DescriptorSet>(*_device, *_currentPool, layout);
		switch (descriptorSet->GetLastResult())
		{
		case VK_SUCCESS:
			return true;
		case VK_ERROR_FRAGMENTED_POOL:
		case VK_ERROR_OUT_OF_POOL_MEMORY:
			_currentPool = GetPool();
			_usedPools.push_back(_currentPool);
			descriptorSet = std::make_shared<DescriptorSet>(*_device, *_currentPool, layout);
			if (descriptorSet->GetLastResult() == VK_SUCCESS)
				return true;
			break;
		default:
			return false;
		}

		return false;
	}

	DescriptorPoolPtr DescriptorAllocator::CreatePool(VkDescriptorPoolCreateFlags flags)
	{
		std::vector<VkDescriptorPoolSize> sizes;
		sizes.reserve(_poolSizes.sizes.size());
		for (auto sz : _poolSizes.sizes)
		{
			sizes.push_back({ sz.first, uint32_t(sz.second * DESCRIPTOR_POOL_SIZE) });
		}
		return std::make_shared<DescriptorPool>(*_device, sizes);
	}

	DescriptorPoolPtr DescriptorAllocator::GetPool()
	{
		if (_freePools.empty())
			return CreatePool(0);
		DescriptorPoolPtr pool = _freePools.back();
		_freePools.pop_back();
		return pool;
	}

	void DescriptorAllocator::Reset()
	{
		for (auto& pool : _usedPools)
		{
			pool->Reset();
			_freePools.push_back(pool);
		}
		_usedPools.clear();
		_currentPool = VK_NULL_HANDLE;
	}

	Device& DescriptorAllocator::GetDevice()
	{
		assert(_device != nullptr);
		return *_device;
	}
}