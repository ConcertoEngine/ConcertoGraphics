//
// Created by arthur on 16/02/2023.
//

#ifndef CONCERTO_GRAPHICS_INCLUDE_DESCRIPTORBUILDER_HPP_
#define CONCERTO_GRAPHICS_INCLUDE_DESCRIPTORBUILDER_HPP_

#include <vector>
#include <memory>

#include <vulkan/vulkan.h>
#include "Concerto/Graphics/Defines.hpp"

namespace Concerto::Graphics
{
	class DescriptorSetLayout;
	class DescriptorSet;
	class DescriptorLayoutCache;
	class DescriptorAllocator;

	class CONCERTO_GRAPHICS_API DescriptorBuilder
	{
	 public:
		DescriptorBuilder(DescriptorLayoutCache& layoutCache, DescriptorAllocator& allocator);

		DescriptorBuilder& BindBuffer(UInt32 binding, VkDescriptorBufferInfo* bufferInfo, VkDescriptorType type, VkShaderStageFlags stageFlags);
		DescriptorBuilder& BindImage(UInt32 binding, VkDescriptorImageInfo* imageInfo, VkDescriptorType type, VkShaderStageFlags stageFlags);

		bool Build(std::shared_ptr<DescriptorSet>& set, std::shared_ptr<DescriptorSetLayout>& layout);
		bool Build(std::shared_ptr<DescriptorSet>& set);
	 private:

		std::vector<VkWriteDescriptorSet> _writes;
		std::vector<VkDescriptorSetLayoutBinding> _bindings;

		DescriptorLayoutCache& _cache;
		DescriptorAllocator& _alloc;
	};
}
#endif //CONCERTO_GRAPHICS_INCLUDE_DESCRIPTORBUILDER_HPP_
