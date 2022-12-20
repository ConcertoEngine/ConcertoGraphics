//
// Created by arthur on 16/06/22.
//

#ifndef CONCERTOGRAPHICS_DESCRIPTORPOOL_HPP
#define CONCERTOGRAPHICS_DESCRIPTORPOOL_HPP

#include <vector>
#include "vulkan/vulkan.h"
#include "wrapper/Object.hpp"
#include "wrapper/DescriptorSet.hpp"

namespace Concerto::Graphics::Wrapper
{
	class Device;


	class DescriptorPool : public Object<VkDescriptorPool>
	{
	public:
		explicit DescriptorPool(Device& device);

		DescriptorPool(Device& device, std::vector<VkDescriptorPoolSize> poolSizes);

		DescriptorPool(DescriptorPool&&) = default;

		DescriptorPool(const DescriptorPool&) = delete;

		DescriptorPool& operator=(DescriptorPool&&) = default;

		DescriptorPool& operator=(const DescriptorPool&) = delete;

		DescriptorSet AllocateDescriptorSet(DescriptorSetLayout& setLayout);
	};
}

#endif //CONCERTOGRAPHICS_DESCRIPTORPOOL_HPP
