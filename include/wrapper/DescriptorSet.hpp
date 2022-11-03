//
// Created by arthur on 16/06/22.
//

#ifndef CONCERTOGRAPHICS_DESCRIPTORSET_HPP
#define CONCERTOGRAPHICS_DESCRIPTORSET_HPP

#include "vulkan/vulkan.h"
#include "wrapper/Object.hpp"

namespace Concerto::Graphics::Wrapper
{
	class DescriptorPool;

	class DescriptorSetLayout;

	class Sampler;

	class ImageView;

	class Device;

	class DescriptorSet : public Object<VkDescriptorSet>
	{
	public:
		DescriptorSet(Device& device, DescriptorPool& pool,
				DescriptorSetLayout& descriptorSetLayout);

		DescriptorSet(DescriptorSet&&) = default;

		DescriptorSet(const DescriptorSet&) = delete;

		DescriptorSet& operator=(DescriptorSet&&) = default;

		DescriptorSet& operator=(const DescriptorSet&) = delete;

		~DescriptorSet() = default;
		
		void WriteImageSamplerDescriptor(Sampler& sampler, ImageView& imageView,
				VkImageLayout imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);
	};
}

#endif //CONCERTOGRAPHICS_DESCRIPTORSET_HPP
