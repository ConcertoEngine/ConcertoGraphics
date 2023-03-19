//
// Created by arthur on 10/06/22.
//

#ifndef CONCERTOGRAPHICS_SHADERMODULE_HPP
#define CONCERTOGRAPHICS_SHADERMODULE_HPP

#include <string>
#include <vector>
#include <vulkan/vulkan.h>
#include <Concerto/Core/Types.hpp>
#include "Object.hpp"

namespace Concerto::Graphics::Wrapper
{
	class Device;

	/**
	 * @class ShaderModule
	 * @brief A class representing a ShaderModule.
	 *
	 * A ShaderModule is a class that encapsulate the creation, loading and handling of a VkShaderModule object.
	 */
	class CONCERTO_PUBLIC_API ShaderModule : public Object<VkShaderModule>
	{
	public:
		/**
		 * @brief Constructs a ShaderModule object.
		 * @param device The Device object associated with the ShaderModule.
		 * @param shaderPath The path to the shader source file.
		 */
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
