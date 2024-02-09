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

	bool DescriptorAllocator::Allocate(DescriptorSetPtr& descriptorSet, const DescriptorSetLayout& layout)
	{
		auto it = _cache.find(layout.GetHash());
		if (it != _cache.end())
		{
			descriptorSet = it->second;
			return true;
		}
		if (auto set = TryAllocate(layout))
		{
			descriptorSet = set;
			_cache.emplace(layout.GetHash(), set);
			return true;
		}
		return false;
	}

	bool DescriptorAllocator::AllocateWithoutCache(DescriptorSetPtr& descriptorSet, const DescriptorSetLayout& layout)
	{
		descriptorSet = TryAllocate(layout);
		return descriptorSet != nullptr;
	}

	DescriptorPoolPtr DescriptorAllocator::CreatePool(VkDescriptorPoolCreateFlags)
	{
		std::vector<VkDescriptorPoolSize> sizes;
		sizes.reserve(_poolSizes.sizes.size());
		for (const auto& [descriptorType, number] : _poolSizes.sizes)
			sizes.push_back({ descriptorType, static_cast<UInt32>(number * DESCRIPTOR_POOL_SIZE) });
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

	DescriptorSetPtr DescriptorAllocator::TryAllocate(const DescriptorSetLayout& layout)
	{
		if (_currentPool == VK_NULL_HANDLE)
		{
			_currentPool = GetPool();
			_usedPools.push_back(_currentPool);
		}
		auto descriptorSet = std::make_shared<DescriptorSet>(*_device, *_currentPool, layout);
		switch (descriptorSet->GetLastResult())
		{
		case VK_SUCCESS:
			return descriptorSet;
		case VK_ERROR_FRAGMENTED_POOL:
		case VK_ERROR_OUT_OF_POOL_MEMORY:
			_currentPool = GetPool();
			_usedPools.push_back(_currentPool);
			descriptorSet = std::make_shared<DescriptorSet>(*_device, *_currentPool, layout);
			if (descriptorSet->GetLastResult() == VK_SUCCESS)
			{
				_cache.emplace(layout.GetHash(), descriptorSet);
				return descriptorSet;
			}
			break;
		default:
			return nullptr;
		}
		return nullptr;
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

	Device& DescriptorAllocator::GetDevice() const
	{
		CONCERTO_ASSERT(_device != nullptr);
		return *_device;
	}

	DescriptorPoolPtr DescriptorAllocator::GetDescriptorPool()
	{
		return _currentPool;
	}
}
