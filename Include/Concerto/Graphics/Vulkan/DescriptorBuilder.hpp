//
// Created by arthur on 16/02/2023.
//

#ifndef CONCERTOGRAPHICS_INCLUDE_DESCRIPTORBUILDER_HPP_
#define CONCERTOGRAPHICS_INCLUDE_DESCRIPTORBUILDER_HPP_

#include <memory>

#include <Concerto/Core/Types.hpp>

#include "Concerto/Graphics/Vulkan/DescriptorLayoutCache.hpp"
#include "Concerto/Graphics/Vulkan/DescriptorAllocator.hpp"
#include "Concerto/Graphics/Vulkan/Wrapper/DescriptorSet.hpp"

namespace Concerto::Graphics
{
	class DescriptorSetLayout;

	class CONCERTO_PUBLIC_API DescriptorBuilder
	{
	 public:
		DescriptorBuilder(DescriptorLayoutCache& layoutCache, DescriptorAllocator& allocator);

		DescriptorBuilder& BindBuffer(UInt32 binding, VkDescriptorBufferInfo* bufferInfo, VkDescriptorType type, VkShaderStageFlags stageFlags);
		DescriptorBuilder& BindImage(UInt32 binding, VkDescriptorImageInfo* imageInfo, VkDescriptorType type, VkShaderStageFlags stageFlags);

		bool Build(DescriptorSetPtr& set, DescriptorSetLayoutPtr& layout);
		bool Build(DescriptorSetPtr& set);
	 private:

		std::vector<VkWriteDescriptorSet> writes;
		std::vector<VkDescriptorSetLayoutBinding> bindings;

		DescriptorLayoutCache& cache;
		DescriptorAllocator& alloc;
	};
}
#endif //CONCERTOGRAPHICS_INCLUDE_DESCRIPTORBUILDER_HPP_
