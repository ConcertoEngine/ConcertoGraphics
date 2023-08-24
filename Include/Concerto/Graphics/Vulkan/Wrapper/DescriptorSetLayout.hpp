//
// Created by arthur on 16/06/22.
//

#ifndef CONCERTOGRAPHICS_DESCRIPTORSETLAYOUT_HPP
#define CONCERTOGRAPHICS_DESCRIPTORSETLAYOUT_HPP

#include <vector>
#include <memory>

#include <vulkan/vulkan.h>
#include "Concerto/Graphics/Defines.hpp"

#include "Concerto/Graphics/Vulkan/Wrapper/Object.hpp"

namespace Concerto::Graphics
{
	class Device;

	/**
	* @class DescriptorSetLayout
	* @brief This class represents a VkDescriptorSetLayout.
	* A descriptor set layout defines the layout of the descriptors within a descriptor set.
	*
	* @note This class is not copyable, but it is movable.
	*/
	class CONCERTO_GRAPHICS_API DescriptorSetLayout : public Object<VkDescriptorSetLayout>
	{
	public:
		DescriptorSetLayout(Device& device, std::vector<VkDescriptorSetLayoutBinding> bindings);

		DescriptorSetLayout(DescriptorSetLayout&&) = default;

		DescriptorSetLayout(const DescriptorSetLayout&) = delete;

		DescriptorSetLayout& operator=(DescriptorSetLayout&&) = default;

		~DescriptorSetLayout() = default;
	};
	using DescriptorSetLayoutPtr = std::shared_ptr<DescriptorSetLayout>;

	DescriptorSetLayoutPtr MakeDescriptorSetLayout(Device& device, const std::vector<VkDescriptorSetLayoutBinding>& bindings);
}

#endif //CONCERTOGRAPHICS_DESCRIPTORSETLAYOUT_HPP
