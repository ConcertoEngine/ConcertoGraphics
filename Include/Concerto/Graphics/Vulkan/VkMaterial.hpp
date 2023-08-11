//
// Created by arthur on 13/09/2022.
//

#ifndef CONCERTOGRAPHICS_MATERIAL_HPP
#define CONCERTOGRAPHICS_MATERIAL_HPP

#include <optional>
#include <memory>
#include <vulkan/vulkan.h>

#include "Concerto/Graphics/Vulkan/Wrapper/DescriptorSet.hpp"

namespace Concerto::Graphics
{
	class VkMaterial
	{
	 public:
		VkMaterial() : _pipelineLayout(VK_NULL_HANDLE),
					   _pipeline(VK_NULL_HANDLE)
		{

		}

		VkMaterial(VkPipelineLayout pipelineLayout, VkPipeline pipeline) : _pipelineLayout(pipelineLayout),
																		   _pipeline(pipeline)
		{
		}

		bool operator==(const VkMaterial& other) const
		{
			return _pipelineLayout == other._pipelineLayout && _pipeline == other._pipeline;
		}

		bool operator!=(const VkMaterial& other) const
		{
			return !(*this == other);
		}

		VkPipeline _pipeline;
		VkPipelineLayout _pipelineLayout;
		DescriptorSetPtr _diffuseTextureSet;
		DescriptorSetPtr _normalTextureSet;
	};
	using VkMaterialPtr = std::shared_ptr<VkMaterial>;
}
#endif //CONCERTOGRAPHICS_MATERIAL_HPP
