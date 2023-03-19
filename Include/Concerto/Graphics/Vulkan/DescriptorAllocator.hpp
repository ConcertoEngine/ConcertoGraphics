//
// Created by arthur on 16/02/2023.
//

#ifndef CONCERTOGRAPHICS_INCLUDE_DESCRIPTORALLOCATOR_HPP_
#define CONCERTOGRAPHICS_INCLUDE_DESCRIPTORALLOCATOR_HPP_

#include <vector>
#include <vulkan/vulkan.h>
#include <Concerto/Core/Types.hpp>
#include "Vulkan/Wrapper/DescriptorPool.hpp"

namespace Concerto::Graphics
{
	namespace Wrapper
	{
		class DescriptorSet;
		class DescriptorSetLayout;
		class Device;
	}
	class CONCERTO_PUBLIC_API DescriptorAllocator
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
		bool Allocate(Wrapper::DescriptorSetPtr &descriptorSet, Wrapper::DescriptorSetLayout &layout);
	 	/**
	 	 * @brief Reset the allocator, resetting all the descriptors in the pool.
	 	 */
		void Reset();
		Wrapper::Device &GetDevice();
	 private:
		Wrapper::DescriptorPoolPtr CreatePool(VkDescriptorPoolCreateFlags flags);
		Wrapper::DescriptorPoolPtr GetPool();
		Wrapper::Device *_device;
		PoolSizes _poolSizes;
		Wrapper::DescriptorPoolPtr _currentPool;
		std::vector<Wrapper::DescriptorPoolPtr> _usedPools;
		std::vector<Wrapper::DescriptorPoolPtr> _freePools;
	};
}

#endif //CONCERTOGRAPHICS_INCLUDE_DESCRIPTORALLOCATOR_HPP_
