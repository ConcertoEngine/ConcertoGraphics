//
// Created by arthur on 16/06/22.
//

#ifndef CONCERTOGRAPHICS_DESCRIPTORSETLAYOUT_HPP
#define CONCERTOGRAPHICS_DESCRIPTORSETLAYOUT_HPP

#include "vulkan/vulkan.h"
#include <vector>

namespace Concerto::Graphics::Wrapper
{
	class DescriptorSetLayout
	{
	public:
		DescriptorSetLayout(VkDevice device, std::vector<VkDescriptorSetLayoutBinding> bindings);

		DescriptorSetLayout(DescriptorSetLayout&&) = default;

		DescriptorSetLayout(const DescriptorSetLayout&) = default;

		DescriptorSetLayout& operator=(DescriptorSetLayout&&) = default;

		DescriptorSetLayout& operator=(const DescriptorSetLayout&) = default;

		~DescriptorSetLayout();
		[[nodiscard]] VkDescriptorSetLayout Get() const;

	private:
		VkDevice _device;
		VkDescriptorSetLayout _layout;
	};
}

#endif //CONCERTOGRAPHICS_DESCRIPTORSETLAYOUT_HPP
