//
// Created by arthur on 28/08/2023.
//

#include "Concerto/Graphics/ShaderModuleInfo.hpp"

#include <fstream>

#include "Concerto/Graphics/ShaderReflection.hpp"
#include "Concerto/Graphics/Vulkan/Wrapper/Device.hpp"
#include "Concerto/Graphics/Vulkan/Wrapper/ShaderModule.hpp"
#include "Concerto/Graphics/Vulkan/Wrapper/VulkanInitializer.hpp"

namespace Concerto::Graphics
{
	ShaderModuleInfo::ShaderModuleInfo(Device& device, std::string_view path) : 
		shaderAst(nzsl::ParseFromFile(path)),
		sanitizedModule(nzsl::Ast::Sanitize(*shaderAst))
	{
		nzsl::Ast::ReflectVisitor::Callbacks callbacks;
		std::vector<VkDescriptorSetLayoutBinding> descriptorSetLayouts;
		VkShaderStageFlags shaderStageFlag = VK_SHADER_STAGE_ALL;
		callbacks.onEntryPointDeclaration = [&](nzsl::ShaderStageType stageType, const std::string& functionName) {
			shaderStageFlag = ToVulkan(stageType);
			entryPointName = functionName;
		};
		//callbacks.onAliasDeclaration = [](const nzsl::Ast::DeclareAliasStatement& aliasDeclaration){};
		//callbacks.onConstDeclaration = [](const nzsl::Ast::DeclareConstStatement& constDecl){};
		callbacks.onExternalDeclaration = [&](const nzsl::Ast::DeclareExternalStatement& extDecl) {
			for (auto& externalVariable : extDecl.externalVars)
			{
				const auto* varType = &externalVariable.type.GetResultingValue();
				const VkDescriptorType descriptorType = GetBindingType(varType);
				UInt32 bindingSet = externalVariable.bindingSet.GetResultingValue();
				auto descriptorSetLayoutBinding = VulkanInitializer::DescriptorSetLayoutBinding(descriptorType, shaderStageFlag,	externalVariable.bindingIndex.GetResultingValue());
				auto layoutBindings = bindings.find(bindingSet);
				if (layoutBindings == bindings.end())
					bindings[bindingSet] = std::vector{ descriptorSetLayoutBinding };
				else
					layoutBindings->second.push_back(descriptorSetLayoutBinding);
			}
		};
		//callbacks.onFunctionDeclaration = [](const nzsl::Ast::DeclareFunctionStatement& funcDecl) {};
		//callbacks.onOptionDeclaration = [](const nzsl::Ast::DeclareOptionStatement& optionDecl) {};
		//callbacks.onStructDeclaration = [](const nzsl::Ast::DeclareStructStatement& structDecl) {};
		//callbacks.onVariableDeclaration = [](const nzsl::Ast::DeclareVariableStatement& variableDecl) {};
		//callbacks.onAliasIndex = [](const std::string& name, std::size_t aliasIndex, const nzsl::SourceLocation& sourceLocation) {};
		//callbacks.onConstIndex = [](const std::string& name, std::size_t constIndex, const nzsl::SourceLocation& sourceLocation) {};
		//callbacks.onFunctionIndex = [](const std::string& name, std::size_t funcIndex, const nzsl::SourceLocation& sourceLocation) {};
		//callbacks.onOptionIndex = [](const std::string& name, std::size_t optIndex, const nzsl::SourceLocation& sourceLocation) {};
		//callbacks.onStructIndex = [](const std::string& name, std::size_t structIndex, const nzsl::SourceLocation& sourceLocation) {};
		//callbacks.onVariableIndex = [](const std::string& name, std::size_t varIndex, const nzsl::SourceLocation& sourceLocation) {};
		nzsl::Ast::ReflectVisitor reflectVisitor;
		reflectVisitor.Reflect(*sanitizedModule, callbacks);

		nzsl::SpirvWriter spirvWriter;
		auto sprivVersion = spirvWriter.GetMaximumSupportedVersion(1, 3);
		nzsl::SpirvWriter::Environment env = {
			.spvMajorVersion = 1,
			.spvMinorVersion = 3
		};
		spirvWriter.SetEnv(env);
		spirv = spirvWriter.Generate(*sanitizedModule);
		shaderModule = std::make_unique<ShaderModule>(device, spirv, static_cast<VkShaderStageFlagBits>(shaderStageFlag), std::move(entryPointName));
	}

	VkDescriptorType ShaderModuleInfo::GetBindingType(const nzsl::Ast::ExpressionType* varType)
	{
		if (IsStorageType(*varType))
			return VK_DESCRIPTOR_TYPE_STORAGE_BUFFER;
		if (IsSamplerType(*varType))
			return VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
		if (IsTextureType(*varType))
			return VK_DESCRIPTOR_TYPE_STORAGE_IMAGE;
		if (IsUniformType(*varType))
			return VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
		CONCERTO_ASSERT_FALSE("ConcertoGraphics: Unexpeted type {}", ToString(varType));
		throw std::runtime_error("unexpected type " + nzsl::Ast::ToString(varType));

	}
}