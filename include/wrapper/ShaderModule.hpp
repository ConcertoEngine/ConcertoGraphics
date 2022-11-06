//
// Created by arthur on 10/06/22.
//

#ifndef CONCERTOGRAPHICS_SHADERMODULE_HPP
#define CONCERTOGRAPHICS_SHADERMODULE_HPP

#include <string>
#include <vector>
#include "vulkan/vulkan.h"
#include "wrapper/Object.hpp"

namespace Concerto::Graphics::Wrapper
{
	class Device;

	class ShaderModule : public Object<VkShaderModule>
	{
	public:
		ShaderModule(Device& device, const std::string& shaderPath);

		ShaderModule(ShaderModule&&) = default;

		ShaderModule(const ShaderModule&) = delete;

		ShaderModule& operator=(ShaderModule&&) = default;

		ShaderModule& operator=(const ShaderModule&) = delete;
	private:
		void loadShaderModule(const std::string& shaderPath);

		void createShaderModule();

		VkShaderModuleCreateInfo _shaderModuleCreateInfo{};
		std::vector<std::uint32_t> _buffer;
	};
}

#endif //CONCERTOGRAPHICS_SHADERMODULE_HPP
