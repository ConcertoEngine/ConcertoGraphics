//
// Created by arthur on 16/02/2023.
//

#ifndef CONCERTOGRAPHICS_INCLUDE_SHADEREFFECT_HPP_
#define CONCERTOGRAPHICS_INCLUDE_SHADEREFFECT_HPP_

#include <array>

#include <vulkan/vulkan.h>

#include "Concerto/Graphics/Vulkan/wrapper/ShaderModule.hpp"

namespace Concerto::Graphics
{
	class ShaderEffect
	{
	 public:

		VkPipelineLayout builtLayout;
		std::array<VkDescriptorSetLayout, 4> setLayouts;

	 private:
		struct ShaderStage
		{
			ShaderModule* shaderModule;
			VkShaderStageFlagBits stage;
		};

		std::vector<ShaderStage> stages;
	};
}
#endif //CONCERTOGRAPHICS_INCLUDE_SHADEREFFECT_HPP_
