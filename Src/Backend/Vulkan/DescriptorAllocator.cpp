//
// Created by arthur on 16/02/2023.
//

#define DESCRIPTOR_POOL_SIZE 1000

#include "Concerto/Graphics/Backend/Vulkan/DescriptorAllocator.hpp"

#include <sstream>

#include "Concerto/Graphics/Backend/Vulkan/Wrapper/DescriptorSet.hpp"
#include "Concerto/Graphics/Backend/Vulkan/Wrapper/DescriptorSetLayout.hpp"
#include "Concerto/Graphics/Backend/Vulkan/Wrapper/Device.hpp"

namespace cct::gfx::vk
{
	DescriptorAllocator::DescriptorAllocator(Device& device) :
		_device(&device)
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
		CCT_GFX_AUTO_PROFILER_SCOPE();
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
		CCT_GFX_AUTO_PROFILER_SCOPE();
		std::lock_guard _(_usedPoolMutex);
		DescriptorPoolPtr currentPool;
		std::shared_ptr<DescriptorSet> descriptorSet;
		auto createDescriptorSet = [&]() -> std::shared_ptr<DescriptorSet>
		{
			return std::make_shared<DescriptorSet>(*_device, *currentPool, layout);
		};

		auto it = _usedPools.find(std::this_thread::get_id());
		if (it == _usedPools.end())
		{
			currentPool = GetPool();
			descriptorSet = createDescriptorSet();
			_usedPools.emplace(std::this_thread::get_id(), std::vector{currentPool});
		}
		else
		{
			// Try to create a descriptor set from an existing pool
			bool succeed = false;
			for (auto& pool : it->second)
			{
				currentPool = pool;
				descriptorSet = createDescriptorSet();
				succeed = descriptorSet && descriptorSet->GetLastResult() == VK_SUCCESS;
				if (succeed)
					break;
			}

			// Create a new pool and allocate a descriptor set within
			if (succeed == false)
			{
				currentPool = GetPool();
				descriptorSet = createDescriptorSet();
				succeed = descriptorSet && descriptorSet->GetLastResult() == VK_SUCCESS;
				if (succeed == false)
				{
					CCT_ASSERT_FALSE("Failed to allocate descriptor set in a newly created pool"); // FIXME
					return nullptr;
				}
				it->second.emplace_back(currentPool);
			}
		}

		if (descriptorSet && descriptorSet->GetLastResult() == VK_SUCCESS)
		{
			_cache.emplace(layout.GetHash(), descriptorSet);
			return descriptorSet;
		}
		return nullptr;
	}

	void DescriptorAllocator::Reset()
	{
		std::lock_guard _(_usedPoolMutex);
		auto it = _usedPools.find(std::this_thread::get_id());
		if (it != _usedPools.end())
		{
			for (auto& pool : it->second)
			{
				pool->Reset();
				_freePools.push_back(pool);
			}
			_usedPools.erase(it);
		}
	}

	Device& DescriptorAllocator::GetDevice() const
	{
		CCT_ASSERT(_device != nullptr, "Vulkan device is null");
		return *_device;
	}

	DescriptorPoolPtr DescriptorAllocator::GetDescriptorPool()
	{
		/*std::lock_guard _(_usedPoolMutex);
		auto it = _usedPools.find(std::this_thread::get_id());
		if (it == _usedPools.end())
			return GetPool();
		return it->second;*/
		CCT_ASSERT_FALSE("FIXME");
		return nullptr;
	}
}
