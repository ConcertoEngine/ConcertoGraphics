//
// Created by arthur on 16/06/22.
//

#ifndef CONCERTOGRAPHICS_DESCRIPTORPOOL_HPP
#define CONCERTOGRAPHICS_DESCRIPTORPOOL_HPP

#include "vulkan/vulkan.h"
#include <vector>
namespace Concerto::Graphics::Wrapper
{
	class DescriptorPool
	{
	public:
		DescriptorPool(VkDevice device, std::vector<VkDescriptorPoolSize> poolSizes);

		DescriptorPool(DescriptorPool&&) = default;

		DescriptorPool(const DescriptorPool&) = default;

		DescriptorPool& operator=(DescriptorPool&&) = default;

		DescriptorPool& operator=(const DescriptorPool&) = default;

		~DescriptorPool();
		VkDescriptorPool get() const;

	private:
		VkDevice _device;
		VkDescriptorPool _pool;
	};
}

#endif //CONCERTOGRAPHICS_DESCRIPTORPOOL_HPP
