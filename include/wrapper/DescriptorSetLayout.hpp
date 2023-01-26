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

	/**
	* @class DescriptorSetLayout
	* @brief This class represents a VkDescriptorSetLayout.
	* A descriptor set layout defines the layout of the descriptors within a descriptor set.
	*
	* @note This class is not copyable, but it is movable.
	*/
	class DescriptorSetLayout : public Object<VkDescriptorSetLayout>
	{
	public:
		DescriptorSetLayout(Device& device, std::vector<VkDescriptorSetLayoutBinding> bindings);

		DescriptorSetLayout(DescriptorSetLayout&&) = default;

		DescriptorSetLayout(const DescriptorSetLayout&) = delete;

		DescriptorSetLayout& operator=(DescriptorSetLayout&&) = default;

		DescriptorSetLayout& operator=(const DescriptorSetLayout&) = delete;

		~DescriptorSetLayout() = default;
	};
}

#endif //CONCERTOGRAPHICS_DESCRIPTORSETLAYOUT_HPP
