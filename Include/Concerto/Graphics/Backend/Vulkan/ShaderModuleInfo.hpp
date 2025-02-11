//
// Created by arthur on 28/08/2023.
//

#ifndef CONCERTO_GRAPHICS_INCLUDE_SHADERMODULEINFO_HPP_
#define CONCERTO_GRAPHICS_INCLUDE_SHADERMODULEINFO_HPP_


#include <NZSL/Parser.hpp>
#include <NZSL/SpirvWriter.hpp>
#include <NZSL/Ast/ReflectVisitor.hpp>

#include "Concerto/Graphics/Backend/Vulkan/Defines.hpp"
#include "Concerto/Graphics/Backend/Vulkan/Wrapper/ShaderModule.hpp"

namespace cct::gfx::vk
{
	class Device;
	class ShaderModule;

	class CONCERTO_GRAPHICS_VULKAN_BACKEND_API ShaderModuleInfo
	{
	public:
		ShaderModuleInfo() = delete;
		ShaderModuleInfo(Device& device, std::string_view path);
		ShaderModuleInfo(const ShaderModuleInfo&) = delete;
		ShaderModuleInfo(ShaderModuleInfo&&) = default;

		ShaderModuleInfo& operator=(const ShaderModuleInfo&) = delete;
		ShaderModuleInfo& operator=(ShaderModuleInfo&&) = default;

		nzsl::Ast::ModulePtr shaderAst;
		nzsl::Ast::ModulePtr sanitizedModule;
		std::vector<UInt32> spirv;
		std::unique_ptr<vk::ShaderModule> shaderModule;
		std::unordered_map<UInt32 /*binding set*/, std::vector<VkDescriptorSetLayoutBinding>> bindings;
		std::string entryPointName;
		VkShaderStageFlagBits stage;
	private:
		VkDescriptorType GetBindingType(const nzsl::Ast::ExpressionType* varType);
	};
}
#endif //CONCERTO_GRAPHICS_INCLUDE_SHADERMODULEINFO_HPP_