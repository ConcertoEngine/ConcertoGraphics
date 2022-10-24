//
// Created by arthur on 13/09/2022.
//

#ifndef CONCERTOGRAPHICS_MATERIAL_HPP
#define CONCERTOGRAPHICS_MATERIAL_HPP

#include "vulkan/vulkan.h"
#include "wrapper/DescriptorSet.hpp"

namespace Concerto::Graphics
{
	struct Material
	{
		Material(VkPipelineLayout pipelineLayout, VkPipeline pipeline) : _pipelineLayout(pipelineLayout),
																		 _pipeline(pipeline)
		{
		}

		Material() : _pipelineLayout(VK_NULL_HANDLE), _pipeline(VK_NULL_HANDLE)
		{
		}

		bool operator==(const Material& other) const
		{
			return _pipelineLayout == other._pipelineLayout && _pipeline == other._pipeline;
		}

		bool operator!=(const Material& other) const
		{
			return !(*this == other);
		}

		VkPipeline _pipeline;
		VkPipelineLayout _pipelineLayout;
		Wrapper::DescriptorSet _textureSet;
	};
}
#endif //CONCERTOGRAPHICS_MATERIAL_HPP
