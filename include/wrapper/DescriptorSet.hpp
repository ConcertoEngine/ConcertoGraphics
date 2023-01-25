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

		DescriptorSet(DescriptorSet&&) noexcept;

		DescriptorSet(const DescriptorSet&) = delete;

		DescriptorSet& operator=(DescriptorSet&&) noexcept = default;

		DescriptorSet& operator=(const DescriptorSet&) = delete;

		void WriteImageSamplerDescriptor(Sampler& sampler, ImageView& imageView,
				VkImageLayout imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);
	private:
		DescriptorPool* _pool{};
	};
}

#endif //CONCERTOGRAPHICS_DESCRIPTORSET_HPP
