//
// Created by arthur on 28/08/2023.
//

#ifndef CONCERTOGRAPHICS_INCLUDE_SHADERMODULEINFO_HPP_
#define CONCERTOGRAPHICS_INCLUDE_SHADERMODULEINFO_HPP_

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
		ShaderModuleInfo(Device& device, std::string_view path);
	
		nzsl::Ast::ModulePtr shaderAst;
		nzsl::Ast::ModulePtr sanitizedModule;
		std::unique_ptr<PipelineLayout> pipelineLayout;
		std::vector<UInt32> spirv;
		std::unique_ptr<ShaderModule> shaderModule;
	private:
		VkDescriptorType GetBindingType(const nzsl::Ast::ExpressionType* expression);
	};
}
#endif //CONCERTOGRAPHICS_INCLUDE_SHADERMODULEINFO_HPP_