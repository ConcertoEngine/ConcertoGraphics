//
// Created by arthur on 16/02/2023.
//

#include "DescriptorAllocator.hpp"
#include "wrapper/DescriptorSet.hpp"
#include "wrapper/DescriptorSetLayout.hpp"
#include "wrapper/Device.hpp"
#define DESCRIPTOR_POOL_SIZE 1000
namespace Concerto::Graphics
{

	DescriptorAllocator::DescriptorAllocator(Wrapper::Device& device) : _device(&device)
	{

	}

	bool DescriptorAllocator::Allocate(Wrapper::DescriptorSet& descriptorSet, Wrapper::DescriptorSetLayout& layout)
	{
		//initialize the currentPool handle if it's null
		if (_currentPool == VK_NULL_HANDLE)
		{

			_currentPool = GetPool();
			usedPools.push_back(_currentPool);
		}

		VkDescriptorSetAllocateInfo allocInfo = {};
		allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
		allocInfo.pNext = nullptr;

		allocInfo.pSetLayouts = layout.Get();
		allocInfo.descriptorPool = _currentPool;
		allocInfo.descriptorSetCount = 1;

		VkResult allocResult = vkAllocateDescriptorSets(*_device->Get(), &allocInfo, descriptorSet.Get());

		switch (allocResult)
		{
		case VK_SUCCESS:
			return true;
		case VK_ERROR_FRAGMENTED_POOL:
		case VK_ERROR_OUT_OF_POOL_MEMORY:
			_currentPool = GetPool();
			usedPools.push_back(_currentPool);
			allocInfo.descriptorPool = _currentPool;

			allocResult = vkAllocateDescriptorSets(*_device->Get(), &allocInfo, descriptorSet.Get());
			if (allocResult == VK_SUCCESS)
			{
				return true;
			}
			break;
		default:
			return false;
		}

		return false;
	}

	VkDescriptorPool DescriptorAllocator::CreatePool(VkDescriptorPoolCreateFlags flags)
	{
		std::vector<VkDescriptorPoolSize> sizes(_poolSizes.sizes.size());
		for (auto sz : _poolSizes.sizes)
		{
			sizes.push_back({ sz.first, uint32_t(sz.second * DESCRIPTOR_POOL_SIZE) });
		}
		VkDescriptorPoolCreateInfo pool_info = {};
		pool_info.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
		pool_info.flags = flags;
		pool_info.maxSets = DESCRIPTOR_POOL_SIZE;
		pool_info.poolSizeCount = (uint32_t)sizes.size();
		pool_info.pPoolSizes = sizes.data();

		VkDescriptorPool descriptorPool;
		vkCreateDescriptorPool(*_device->Get(), &pool_info, nullptr, &descriptorPool);

		return descriptorPool;
	}

	VkDescriptorPool DescriptorAllocator::GetPool()
	{
		if (freePools.empty())
			return CreatePool(0);
		VkDescriptorPool pool = freePools.back();
		freePools.pop_back();
		return pool;
	}

	void DescriptorAllocator::Reset()
	{
		for (auto pool : usedPools){
			vkResetDescriptorPool(*_device->Get(), pool, 0);
			freePools.push_back(pool);
		}
		usedPools.clear();
		_currentPool = VK_NULL_HANDLE;
	}

	Wrapper::Device& DescriptorAllocator::GetDevice()
	{
		assert(_device != nullptr);
		return *_device;
	}
}