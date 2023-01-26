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


	/**
	* @class DescriptorPool
	* @brief Represents a VkDescriptorPool object.
	*
	* DescriptorPool class is a wrapper for VkDescriptorPool, it encapsulate the VkDescriptorPool object,
	* it also provide some utility functions for creating the VkDescriptorPool, allocating descriptor sets
	* from it.
	* A VkDescriptorPool is used to allocate descriptor sets from.
	*/

	class DescriptorPool : public Object<VkDescriptorPool>
	{
	public:
		explicit DescriptorPool(Device& device);

		DescriptorPool(Device& device, std::vector<VkDescriptorPoolSize> poolSizes);

		DescriptorPool(DescriptorPool&&) = default;

		DescriptorPool(const DescriptorPool&) = delete;

		DescriptorPool& operator=(DescriptorPool&&) = default;

		DescriptorPool& operator=(const DescriptorPool&) = delete;

		/*
		 * @brief Allocate a descriptor set from the descriptor pool.
		 * @param layout The descriptor set layout used to allocate the descriptor set.
		 */
		DescriptorSet AllocateDescriptorSet(DescriptorSetLayout& setLayout);
	};
}

#endif //CONCERTOGRAPHICS_DESCRIPTORPOOL_HPP
