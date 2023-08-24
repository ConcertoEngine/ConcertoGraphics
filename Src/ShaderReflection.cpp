//
// Created by arthur on 17/08/2023.
//

#include "Concerto/Graphics/ShaderReflection.hpp"
#include "Concerto/Graphics/Vulkan/Wrapper/PipelineLayout.hpp"
#include "Concerto/Graphics/Vulkan/Wrapper/Device.hpp"
#include "Concerto/Graphics/Vulkan/Wrapper/VulkanInitializer.hpp"
#include "Concerto/Graphics/Vulkan/Wrapper/DescriptorSet.hpp"
#include "Concerto/Graphics/Vulkan/Wrapper/DescriptorSetLayout.hpp"

namespace Concerto::Graphics
{
	void ShaderReflection::Reflect(nzsl::Ast::Module& module)
	{
		for (auto& importedModule : module.importedModules)
			importedModule.module->rootNode->Visit(*this);

		module.rootNode->Visit(*this);
	}

	PipelineLayout ShaderReflection::BuildPipelineLayout(Device& device) const
	{
		std::vector<VkDescriptorSetLayoutBinding> bindings(_pipelineLayoutInfo.bindings.size());

		for (std::size_t i = 0; i < _pipelineLayoutInfo.bindings.size(); ++i)
		{
			const auto& binding = _pipelineLayoutInfo.bindings[i];

			bindings[i].binding = binding.bindingIndex;
			bindings[i].descriptorCount = binding.arraySize;
			bindings[i].descriptorType = ToVulkan(binding.type);
			bindings[i].stageFlags = ToVulkan(binding.shaderStageFlags);
			bindings[i].pImmutableSamplers = nullptr;
		}

		UInt32 setCount = 0;
		for (const auto& bindingInfo : _pipelineLayoutInfo.bindings)
			setCount = std::max(setCount, bindingInfo.setIndex + 1);
		
		std::vector<VkDescriptorSetLayout> setLayouts(setCount);
		std::vector<VkDescriptorSetLayoutBinding> setLayoutInfo(setCount);

		for (const auto& bindingInfo : _pipelineLayoutInfo.bindings)
		{
			VkDescriptorSetLayoutBinding& layoutBinding = setLayoutInfo.emplace_back();
			layoutBinding.binding = bindingInfo.bindingIndex;
			layoutBinding.descriptorCount = bindingInfo.arraySize;
			layoutBinding.descriptorType = ToVulkan(bindingInfo.type);
			layoutBinding.pImmutableSamplers = nullptr;
			layoutBinding.stageFlags = ToVulkan(bindingInfo.shaderStageFlags);
		}
		std::vector<DescriptorSetLayoutPtr> _descriptorSetLayouts(setCount);
		for (UInt32 i = 0; i < setCount; ++i)
		{
			_descriptorSetLayouts[i] = MakeDescriptorSetLayout(device, {setLayoutInfo[i]});
			setLayouts[i] = *_descriptorSetLayouts[i]->Get();
		}
		return PipelineLayout(device, setLayouts);
	}

	void ShaderReflection::Visit(nzsl::Ast::ConditionalStatement& node)
	{
		bool previousConditional = _isConditional;
		_isConditional = true;

		RecursiveVisitor::Visit(node);

		_isConditional = previousConditional;
	}

	void ShaderReflection::Visit(nzsl::Ast::DeclareExternalStatement& node)
	{
		ExternalBlockData* externalBlock = nullptr;
		if (!node.tag.empty())
		{
			if (_externalBlocks.find(node.tag) != _externalBlocks.end())
				throw std::runtime_error("duplicate tag " + node.tag);

			externalBlock = &_externalBlocks[node.tag];
		}

		if (_isConditional)
			throw std::runtime_error("external block " + node.tag + " condition must be resolved");

		for (auto& externalVar : node.externalVars)
		{
			UInt32 bindingIndex = externalVar.bindingIndex.GetResultingValue();
			UInt32 bindingSet = externalVar.bindingSet.GetResultingValue();

			ShaderBindingType bindingType;
			UInt32 arraySize = 1;
			const auto* varType = &externalVar.type.GetResultingValue();

			if (IsStorageType(*varType))
				bindingType = ShaderBindingType::StorageBuffer;
			else if (IsSamplerType(*varType))
				bindingType = ShaderBindingType::Sampler;
			else if (IsTextureType(*varType))
				bindingType = ShaderBindingType::Texture;
			else if (IsUniformType(*varType))
				bindingType = ShaderBindingType::UniformBuffer;
			else if (IsArrayType(*varType))
			{
				const auto& arrayType = std::get<nzsl::Ast::ArrayType>(*varType);
				const auto& innerType = arrayType.containedType->type;
				if (!IsSamplerType(innerType))
					throw std::runtime_error("unexpected type " + nzsl::Ast::ToString(innerType) + " in array " + nzsl::Ast::ToString(arrayType));

				arraySize = arrayType.length;
				bindingType = ShaderBindingType::Sampler;
				varType = &innerType;
			}
			else
				throw std::runtime_error("unexpected type " + nzsl::Ast::ToString(varType));


			_pipelineLayoutInfo.bindings.push_back({
				bindingSet,
				bindingIndex,
				arraySize,
				bindingType,
				nzsl::ShaderStageType_All
				});

			if (!externalVar.tag.empty() && externalBlock)
			{
				switch (bindingType)
				{
				case ShaderBindingType::Sampler:
				{
					if (externalBlock->samplers.find(externalVar.tag) != externalBlock->samplers.end())
						throw std::runtime_error("duplicate sampler tag " + externalVar.tag + " in external block " + node.tag);

					const auto& samplerType = std::get<nzsl::Ast::SamplerType>(*varType);

					ExternalSampler& texture = externalBlock->samplers[externalVar.tag];
					texture.bindingIndex = bindingIndex;
					texture.bindingSet = bindingSet;
					texture.imageType = samplerType.dim;
					texture.sampledType = samplerType.sampledType;
					break;
				}

				case ShaderBindingType::StorageBuffer:
				{
					if (externalBlock->storageBlocks.find(externalVar.tag) != externalBlock->storageBlocks.end())
						throw std::runtime_error("duplicate storage buffer tag " + externalVar.tag + " in external block " + node.tag);

					ExternalStorageBlock& storageBuffer = externalBlock->storageBlocks[externalVar.tag];
					storageBuffer.bindingIndex = bindingIndex;
					storageBuffer.bindingSet = bindingSet;
					storageBuffer.structIndex = std::get<nzsl::Ast::StorageType>(*varType).containedType.structIndex;
					break;
				}

				case ShaderBindingType::Texture:
				{
					if (externalBlock->textures.find(externalVar.tag) != externalBlock->textures.end())
						throw std::runtime_error("duplicate textures tag " + externalVar.tag + " in external block " + node.tag);

					const auto& textureType = std::get<nzsl::Ast::TextureType>(*varType);

					ExternalTexture& texture = externalBlock->textures[externalVar.tag];
					texture.bindingIndex = bindingIndex;
					texture.bindingSet = bindingSet;
					texture.accessPolicy = textureType.accessPolicy;
					texture.baseType = textureType.baseType;
					texture.imageFormat = textureType.format;
					texture.imageType = textureType.dim;
					break;
				}

				case ShaderBindingType::UniformBuffer:
				{
					if (externalBlock->uniformBlocks.find(externalVar.tag) != externalBlock->uniformBlocks.end())
						throw std::runtime_error("duplicate storage buffer tag " + externalVar.tag + " in external block " + node.tag);

					ExternalUniformBlock& uniformBuffer = externalBlock->uniformBlocks[externalVar.tag];
					uniformBuffer.bindingIndex = bindingIndex;
					uniformBuffer.bindingSet = bindingSet;
					uniformBuffer.structIndex = std::get<nzsl::Ast::UniformType>(*varType).containedType.structIndex;
					break;
				}
				}
			}
		}
	}

	void ShaderReflection::Visit(nzsl::Ast::DeclareOptionStatement& node)
	{
		if (!node.optType.IsResultingValue())
			throw std::runtime_error("option " + node.optName + " has unresolved type " + node.optName);

		if (_isConditional)
			throw std::runtime_error("option " + node.optName + " condition must be resolved");

		OptionData& optionData = _options[node.optName];
		optionData.hash = Nz::CRC32(node.optName);
		optionData.type = node.optType.GetResultingValue();
	}

	void ShaderReflection::Visit(nzsl::Ast::DeclareStructStatement& node)
	{
		if (!node.description.layout.HasValue())
			return;

		if (node.description.layout.GetResultingValue() != nzsl::Ast::MemoryLayout::Std140)
			throw std::runtime_error("unexpected layout for struct " + node.description.name);

		if (node.description.isConditional || _isConditional)
			throw std::runtime_error("struct " + node.description.name + " condition must be resolved");

		StructData structData(nzsl::StructLayout::Std140);

		for (auto& member : node.description.members)
		{
			if (member.cond.HasValue() && !member.cond.IsResultingValue())
				throw std::runtime_error("unresolved member " + member.name + " condition in struct " + node.description.name);

			if (!member.type.IsResultingValue())
				throw std::runtime_error("unresolved member " + member.name + " type in struct " + node.description.name);

			std::size_t offset = nzsl::Ast::RegisterStructField(structData.fieldOffsets, member.type.GetResultingValue(), [&](std::size_t structIndex) -> const nzsl::FieldOffsets&
				{
					auto it = _structs.find(structIndex);
					if (it == _structs.end())
						throw std::runtime_error("struct #" + std::to_string(structIndex) + " not found");

					return it->second.fieldOffsets;
				});

			std::size_t size = structData.fieldOffsets.GetSize() - offset;

			if (member.tag.empty())
				continue;

			if (structData.members.find(member.tag) != structData.members.end())
				throw std::runtime_error("duplicate struct member tag " + member.tag + " in struct " + node.description.name);

			auto& structMember = structData.members[member.tag];
			structMember.offset = offset;
			structMember.size = size;
			structMember.type = member.type.GetResultingValue();
		}

		_structs.emplace(node.structIndex.value(), std::move(structData));
	}
}