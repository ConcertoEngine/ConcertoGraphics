//
// Created by arthur on 10/06/22.
//

#ifndef CONCERTO_GRAPHICS_SHADERMODULE_HPP
#define CONCERTO_GRAPHICS_SHADERMODULE_HPP

#include <string>
#include <vector>

#include <vulkan/vulkan.h>

#include "Concerto/Graphics/Defines.hpp"
#include "Concerto/Graphics/Vulkan/Wrapper/Object.hpp"

namespace Concerto::Graphics
{
	class Device;

	/**
	 * @class ShaderModule
	 * @brief A class representing a ShaderModule.
	 *
	 * A ShaderModule is a class that encapsulate the creation, loading and handling of a VkShaderModule object.
	 */
	class CONCERTO_GRAPHICS_API ShaderModule : public Object<VkShaderModule>
	{
	public:
		/**
		 * @brief Constructs a ShaderModule object.
		 * @param device The Device object associated with the ShaderModule.
		 * @param shaderPath The path to the shader source file.
		 */
		ShaderModule(Device& device, const std::string& shaderPath);

		ShaderModule(Device& device, const std::vector<UInt32>& bytes);

		ShaderModule(ShaderModule&&) = default;

		ShaderModule(const ShaderModule&) = delete;

		ShaderModule& operator=(ShaderModule&&) = default;

		ShaderModule& operator=(const ShaderModule&) = delete;
	private:
		void loadShaderModule(const std::string& shaderPath);

		void createShaderModule();

		VkShaderModuleCreateInfo _shaderModuleCreateInfo{};
		std::vector<UInt32> _buffer;
	};
}

#endif //CONCERTO_GRAPHICS_SHADERMODULE_HPP
