//
// Created by arthur on 16/06/22.
//

#ifndef CONCERTOGRAPHICS_DESCRIPTORPOOL_HPP
#define CONCERTOGRAPHICS_DESCRIPTORPOOL_HPP

#include <vector>
#include <memory>

#include <vulkan/vulkan.h>
#include "Concerto/Graphics/Defines.hpp"

#include "Concerto/Graphics/Vulkan/Wrapper/Object.hpp"
#include "Concerto/Graphics/Vulkan/Wrapper/DescriptorSet.hpp"

namespace Concerto::Graphics
{
	class Device;

	/**
	* @class DescriptorPool
	* @brief Represents a VkDescriptorPool object.
	*
	* DescriptorPool class is a Wrapper for VkDescriptorPool, it encapsulate the VkDescriptorPool object,
	* it also provide some utility functions for creating the VkDescriptorPool, allocating descriptor sets
	* from it.
	* A VkDescriptorPool is used to allocate descriptor sets from.
	*/

	class CONCERTO_GRAPHICS_API DescriptorPool : public Object<VkDescriptorPool>
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

		/*
		 * @brief Resetting a descriptor pool recycles all of the resources from all of the descriptor sets allocated
		 * from the descriptor pool back to the descriptor pool, and the descriptor sets are implicitly freed.
		 */
		void Reset();
	};
	using DescriptorPoolPtr = std::shared_ptr<DescriptorPool>;
}

#endif //CONCERTOGRAPHICS_DESCRIPTORPOOL_HPP
