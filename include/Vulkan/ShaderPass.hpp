//
// Created by arthur on 16/02/2023.
//

#ifndef CONCERTOGRAPHICS_INCLUDE_SHADERPASS_HPP_
#define CONCERTOGRAPHICS_INCLUDE_SHADERPASS_HPP_

#include <vulkan/vulkan.h>
#include "ShaderEffect.hpp"

namespace Concerto::Graphics
{
	class ShaderPass
	{
		ShaderEffect* effect{ nullptr };
		VkPipeline pipeline{ VK_NULL_HANDLE };
		VkPipelineLayout layout{ VK_NULL_HANDLE };
	};
}
#endif //CONCERTOGRAPHICS_INCLUDE_SHADERPASS_HPP_
