//
// Created by arthur on 16/06/22.
//

#ifndef CONCERTOGRAPHICS_DESCRIPTORSET_HPP
#define CONCERTOGRAPHICS_DESCRIPTORSET_HPP

#include "vulkan/vulkan.h"

namespace Concerto::Graphics::Wrapper
{
	class DescriptorPool;

	class DescriptorSetLayout;

	class Sampler;

	class ImageView;

	class DescriptorSet
	{
	public:
		DescriptorSet() = default;

		DescriptorSet(VkDevice device, DescriptorPool& pool,
				DescriptorSetLayout& descriptorSetLayout);

		DescriptorSet(DescriptorSet&&) = default;

		DescriptorSet(const DescriptorSet&) = default;

		DescriptorSet& operator=(DescriptorSet&&) = default;

		DescriptorSet& operator=(const DescriptorSet&) = default;

		~DescriptorSet() = default;

		VkDescriptorSet* Get();

		[[nodiscard]] bool IsValid() const;

		void WriteImageSamplerDescriptor(Sampler& sampler, ImageView& imageView,
				VkImageLayout imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);

	private:
		VkDescriptorSet _set = { VK_NULL_HANDLE };
		VkDevice _device;
	};
}

#endif //CONCERTOGRAPHICS_DESCRIPTORSET_HPP
