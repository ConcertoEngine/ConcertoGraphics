//
// Created by arthur on 16/02/2023.
//

#ifndef CONCERTOGRAPHICS_INCLUDE_DESCRIPTORALLOCATOR_HPP_
#define CONCERTOGRAPHICS_INCLUDE_DESCRIPTORALLOCATOR_HPP_

#include <vulkan/vulkan.h>
#include <vector>

namespace Concerto::Graphics
{
	namespace Wrapper
	{
		class DescriptorSet;
		class DescriptorSetLayout;
		class Device;
	}
	class DescriptorAllocator
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
		explicit DescriptorAllocator(Wrapper::Device &device);
		bool Allocate(Wrapper::DescriptorSet &descriptorSet, Wrapper::DescriptorSetLayout &layout);
	 	/**
	 	 * @brief Reset the allocator, resetting all the descriptors in the pool.
	 	 */
		void Reset();
		Wrapper::Device &GetDevice();
	 private:
		VkDescriptorPool CreatePool(VkDescriptorPoolCreateFlags flags);
		VkDescriptorPool GetPool();
		Wrapper::Device *_device;
		PoolSizes _poolSizes;
		VkDescriptorPool _currentPool;
		std::vector<VkDescriptorPool> usedPools;
		std::vector<VkDescriptorPool> freePools;
	};
}

#endif //CONCERTOGRAPHICS_INCLUDE_DESCRIPTORALLOCATOR_HPP_
