//
// Created by arthur on 16/02/2023.
//

#ifndef CONCERTOGRAPHICS_INCLUDE_DESCRIPTORBUILDER_HPP_
#define CONCERTOGRAPHICS_INCLUDE_DESCRIPTORBUILDER_HPP_

#include "DescriptorLayoutCache.hpp"
#include "DescriptorAllocator.hpp"

namespace Concerto::Graphics
{
	namespace Wrapper
	{
		class DescriptorSet;
		class DescriptorSetLayout;
	}
	class DescriptorBuilder
	{
	 public:
		DescriptorBuilder(DescriptorLayoutCache& layoutCache, DescriptorAllocator& allocator );

		DescriptorBuilder& BindBuffer(std::uint32_t binding, VkDescriptorBufferInfo* bufferInfo, VkDescriptorType type, VkShaderStageFlags stageFlags);
		DescriptorBuilder& BindImage(std::uint32_t binding, VkDescriptorImageInfo* imageInfo, VkDescriptorType type, VkShaderStageFlags stageFlags);

		bool Build(Wrapper::DescriptorSet& set, Wrapper::DescriptorSetLayoutPtr& layout);
		bool Build(Wrapper::DescriptorSet& set);
	 private:

		std::vector<VkWriteDescriptorSet> writes;
		std::vector<VkDescriptorSetLayoutBinding> bindings;

		DescriptorLayoutCache& cache;
		DescriptorAllocator& alloc;
	};
}
#endif //CONCERTOGRAPHICS_INCLUDE_DESCRIPTORBUILDER_HPP_