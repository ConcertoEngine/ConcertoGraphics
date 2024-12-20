//
// Created by arthur on 16/06/22.
//

#ifndef CONCERTO_GRAPHICS_DESCRIPTORPOOL_HPP
#define CONCERTO_GRAPHICS_DESCRIPTORPOOL_HPP

#include <vector>
#include <memory>


#include "Concerto/Graphics/Backend/Vulkan/Defines.hpp"
#include "Concerto/Graphics/Backend/Vulkan/Wrapper/Object.hpp"
#include "Concerto/Graphics/Backend/Vulkan/Wrapper/DescriptorSet.hpp"

namespace cct::gfx::vk
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

	class CONCERTO_GRAPHICS_VULKAN_BACKEND_API DescriptorPool : public Object<VkDescriptorPool>
	{
	 public:
		explicit DescriptorPool(Device& device);

		DescriptorPool(Device& device, std::vector<VkDescriptorPoolSize> poolSizes);

		DescriptorPool(DescriptorPool&&) = default;

		DescriptorPool(const DescriptorPool&) = delete;

		DescriptorPool& operator=(DescriptorPool&&) = default;

		DescriptorPool& operator=(const DescriptorPool&) = delete;

		~DescriptorPool();

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

#endif //CONCERTO_GRAPHICS_DESCRIPTORPOOL_HPP
