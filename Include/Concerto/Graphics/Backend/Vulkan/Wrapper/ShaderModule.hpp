//
// Created by arthur on 10/06/22.
//

#ifndef CONCERTO_GRAPHICS_SHADERMODULE_HPP
#define CONCERTO_GRAPHICS_SHADERMODULE_HPP

#include <string>
#include <vector>



#include "Concerto/Graphics/Backend/Vulkan/Defines.hpp"
#include "Concerto/Graphics/Backend/Vulkan/Wrapper/Object.hpp"

namespace cct::gfx::vk
{
	class Device;

	class CONCERTO_GRAPHICS_VULKAN_BACKEND_API ShaderModule : public Object<VkShaderModule>
	{
	public:
		ShaderModule(Device& device, const std::string& shaderPath, VkShaderStageFlagBits stageFlags, std::string entryPoint = "main");
		ShaderModule(Device& device, const std::vector<UInt32>& bytes, VkShaderStageFlagBits stageFlags, std::string entryPoint = "main");

		~ShaderModule() override;

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
