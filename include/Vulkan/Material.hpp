//
// Created by arthur on 13/09/2022.
//

#ifndef CONCERTOGRAPHICS_MATERIAL_HPP
#define CONCERTOGRAPHICS_MATERIAL_HPP

#include <optional>
#include "vulkan/vulkan.h"
#include "Vulkan/wrapper/DescriptorSet.hpp"

namespace Concerto::Graphics
{
	class Material
	{
	public:
		Material(VkPipelineLayout pipelineLayout, VkPipeline pipeline) : _pipelineLayout(pipelineLayout),
																		 _pipeline(pipeline)
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
		std::optional<Wrapper::DescriptorSet> _textureSet;
	};
}
#endif //CONCERTOGRAPHICS_MATERIAL_HPP
