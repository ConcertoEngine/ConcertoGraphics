//
// Created by arthur on 10/06/22.
//

#ifndef CONCERTOGRAPHICS_SHADERMODULE_HPP
#define CONCERTOGRAPHICS_SHADERMODULE_HPP

#include <string>
#include <vector>
#include "vulkan/vulkan.h"

namespace Concerto::Graphics::Wrapper
{
	class ShaderModule
	{
	public:
		ShaderModule(const std::string& shaderPath, VkDevice device);

		ShaderModule(ShaderModule&&) = default;

		ShaderModule(const ShaderModule&) = default;

		ShaderModule& operator=(ShaderModule&&) = default;

		ShaderModule& operator=(const ShaderModule&) = default;

		~ShaderModule();

		VkShaderModule getShaderModule();

	private:
		void loadShaderModule(const std::string& shaderPath);

		void createShaderModule();

		VkShaderModule _shaderModule;
		VkShaderModuleCreateInfo _shaderModuleCreateInfo{};
		std::vector<std::uint32_t> _buffer;
		VkDevice _device{};
	};
}

#endif //CONCERTOGRAPHICS_SHADERMODULE_HPP
