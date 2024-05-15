//
// Created by arthur on 10/06/22.
//

#ifndef CONCERTO_GRAPHICS_SHADERMODULE_HPP
#define CONCERTO_GRAPHICS_SHADERMODULE_HPP

#include <string>
#include <vector>

#include <vulkan/vulkan.h>

#include "Concerto/Graphics/Defines.hpp"
#include "Concerto/Graphics/Backend/Vulkan/Wrapper/Object.hpp"

namespace Concerto::Graphics::Vk
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
		 * @param stageFlags The stage flags of the shader.
		 * @param entryPoint The entry point name of the shader.
		 */
		ShaderModule(Device& device, const std::string& shaderPath, VkShaderStageFlagBits stageFlags, std::string entryPoint = "main");

		ShaderModule(Device& device, const std::vector<UInt32>& bytes, VkShaderStageFlagBits stageFlags, std::string entryPoint = "main");

		~ShaderModule();

		ShaderModule(ShaderModule&&) = default;

		ShaderModule(const ShaderModule&) = delete;

		ShaderModule& operator=(ShaderModule&&) = default;

		ShaderModule& operator=(const ShaderModule&) = delete;

		[[nodiscard]] VkPipelineShaderStageCreateInfo GetPipelineShaderStageCreateInfo() const;
	private:
		void LoadShaderModule(const std::string& shaderPath);

		void CreateShaderModule();

		VkShaderModuleCreateInfo _shaderModuleCreateInfo{};
		std::vector<UInt32> _buffer;
		VkShaderStageFlagBits _stageFlags;
		std::string _entryPoint;

	};
}

#endif //CONCERTO_GRAPHICS_SHADERMODULE_HPP
