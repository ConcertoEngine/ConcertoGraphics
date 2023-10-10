//
// Created by arthur on 13/09/2022.
//

#ifndef CONCERTOGRAPHICS_MATERIAL_HPP
#define CONCERTOGRAPHICS_MATERIAL_HPP

#include <optional>
#include <memory>
#include <vector>

#include <vulkan/vulkan.h>

#include "Concerto/Graphics/Vulkan/Wrapper/DescriptorSet.hpp"
#include "Concerto/Graphics/Vulkan/Wrapper/Buffer.hpp"
#include "Concerto/Graphics/Vulkan/Wrapper/Pipeline.hpp"

namespace Concerto::Graphics
{
	struct CONCERTO_GRAPHICS_API DescriptoSetInfo
	{
		DescriptoSetInfo(DescriptorSet descriptorSet) :
			descriptorSet(std::move(descriptorSet)), buffer(nullptr) { }
		//Buffer(_device.GetAllocator(), 0, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, VMA_MEMORY_USAGE_CPU_TO_GPU)
		DescriptoSetInfo(const DescriptoSetInfo&) = delete;
		DescriptoSetInfo(DescriptoSetInfo&&) = default;
		DescriptoSetInfo& operator=(const DescriptoSetInfo&) = delete;
		DescriptoSetInfo& operator=(DescriptoSetInfo&&) = default;

		DescriptorSet descriptorSet;
		std::unique_ptr<Buffer> buffer;
	};
	using DescriptoSetInfoPtr = std::shared_ptr<DescriptoSetInfo>;

	struct CONCERTO_GRAPHICS_API VkMaterial
	{
		std::shared_ptr<Pipeline> _pipeline;
		PipelineLayout* _pipelineLayout;
		std::vector<DescriptoSetInfoPtr> _descriptorSets;
		DescriptorSetPtr _diffuseTextureSet;
		DescriptorSetPtr _normalTextureSet;
	};
	using VkMaterialPtr = std::shared_ptr<VkMaterial>;
}
#endif //CONCERTOGRAPHICS_MATERIAL_HPP
