//
// Created by arthur on 28/08/2023.
//

#ifndef CONCERTO_GRAPHICS_INCLUDE_SHADERMODULEINFO_HPP_
#define CONCERTO_GRAPHICS_INCLUDE_SHADERMODULEINFO_HPP_

#include <string_view>

#include <NZSL/Parser.hpp>
#include <NZSL/SpirvWriter.hpp>
#include <NZSL/Ast/ReflectVisitor.hpp>
#include <NZSL/Ast/SanitizeVisitor.hpp>

#include "Concerto/Graphics/Vulkan/Wrapper/PipelineLayout.hpp"

namespace Concerto::Graphics
{
	class Device;
	class ShaderModule;

	class CONCERTO_GRAPHICS_API ShaderModuleInfo
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
		std::unique_ptr<ShaderModule> shaderModule;
		std::unordered_map<UInt32 /*binding set*/, std::vector<VkDescriptorSetLayoutBinding>> bindings;
		std::string entryPointName;
		VkShaderStageFlagBits stage;
	private:
		VkDescriptorType GetBindingType(const nzsl::Ast::ExpressionType* varType);
	};
}
#endif //CONCERTO_GRAPHICS_INCLUDE_SHADERMODULEINFO_HPP_