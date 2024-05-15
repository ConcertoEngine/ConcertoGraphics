//
// Created by arthur on 16/02/2023.
//

#ifndef CONCERTO_GRAPHICS_INCLUDE_DESCRIPTORALLOCATOR_HPP_
#define CONCERTO_GRAPHICS_INCLUDE_DESCRIPTORALLOCATOR_HPP_

#include <vector>

#include <vulkan/vulkan.h>

#include "Concerto/Graphics/Defines.hpp"
#include "Concerto/Graphics/Backend/Vulkan/Wrapper/DescriptorPool.hpp"

namespace Concerto::Graphics::Vk
{
	class DescriptorSet;
	class DescriptorSetLayout;
	class Device;

	class CONCERTO_GRAPHICS_API DescriptorAllocator
	{
	 public:
		struct PoolSizes {
			std::vector<std::pair<VkDescriptorType,float>> sizes =
				{
					{ VK_DESCRIPTOR_TYPE_SAMPLER, 0.5f },
					{ VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 4.f },
					{ VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE, 4.f },
					{ VK_DESCRIPTOR_TYPE_STORAGE_IMAGE, 1.f },
					{ VK_DESCRIPTOR_TYPE_UNIFORM_TEXEL_BUFFER, 1.f },
					{ VK_DESCRIPTOR_TYPE_STORAGE_TEXEL_BUFFER, 1.f },
					{ VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 2.f },
					{ VK_DESCRIPTOR_TYPE_STORAGE_BUFFER, 2.f },
					{ VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC, 1.f },
					{ VK_DESCRIPTOR_TYPE_STORAGE_BUFFER_DYNAMIC, 1.f },
					{ VK_DESCRIPTOR_TYPE_INPUT_ATTACHMENT, 0.5f }
				};
		};
		explicit DescriptorAllocator(Device &device);
		bool Allocate(DescriptorSetPtr &descriptorSet, const DescriptorSetLayout &layout);
		bool AllocateWithoutCache(DescriptorSetPtr& descriptorSet, const DescriptorSetLayout& layout);
	 	/**
	 	 * @brief Reset the allocator, resetting all the descriptors in the pool.
	 	 */
		void Reset();
		[[nodiscard]] Device &GetDevice() const;
		DescriptorPoolPtr GetDescriptorPool();

	private:
		DescriptorPoolPtr CreatePool(VkDescriptorPoolCreateFlags flags);
		DescriptorPoolPtr GetPool();

		DescriptorSetPtr TryAllocate(const DescriptorSetLayout& layout);
		Device *_device;
		PoolSizes _poolSizes;
		DescriptorPoolPtr _currentPool;
		std::vector<DescriptorPoolPtr> _usedPools;
		std::vector<DescriptorPoolPtr> _freePools;
		std::unordered_map<UInt64 /*Hash of bindings*/, DescriptorSetPtr> _cache;
	};
}

#endif //CONCERTO_GRAPHICS_INCLUDE_DESCRIPTORALLOCATOR_HPP_
