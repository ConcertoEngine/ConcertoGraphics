//
// Created by arthur on 16/06/22.
//

#ifndef CONCERTOGRAPHICS_DESCRIPTORSETLAYOUT_HPP
#define CONCERTOGRAPHICS_DESCRIPTORSETLAYOUT_HPP

#include <vector>
#include "vulkan/vulkan.h"
#include "wrapper/Object.hpp"

namespace Concerto::Graphics::Wrapper
{
	class Device;
	class DescriptorSetLayout : public Object<VkDescriptorSetLayout>
	{
	public:
		DescriptorSetLayout(Device& device, std::vector<VkDescriptorSetLayoutBinding> bindings);

		DescriptorSetLayout(DescriptorSetLayout&&) = default;

		DescriptorSetLayout(const DescriptorSetLayout&) = delete;

		DescriptorSetLayout& operator=(DescriptorSetLayout&&) = default;

		DescriptorSetLayout& operator=(const DescriptorSetLayout&) = delete;

		~DescriptorSetLayout();
	};
}

#endif //CONCERTOGRAPHICS_DESCRIPTORSETLAYOUT_HPP
