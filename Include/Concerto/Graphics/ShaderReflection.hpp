//
// Created by arthur on 23/08/2023.
//

#ifndef CONCERTOGRAPHICS_INCLUDE_SHADERREFLECTION_HPP_
#define CONCERTOGRAPHICS_INCLUDE_SHADERREFLECTION_HPP_

#include <unordered_map>
#include <vector>

#include <Concerto/Core/Types.hpp>
#include <vulkan/vulkan.h>
#include <NZSL/Ast/RecursiveVisitor.hpp>
#include <NZSL/Math/FieldOffsets.hpp>
#include <NZSL/Ast/Module.hpp>

namespace Concerto::Graphics
{
	class PipelineLayout;
	class Device;

	enum class ShaderBindingType
	{
		Sampler,
		StorageBuffer,
		Texture,
		UniformBuffer,

		Max = UniformBuffer
	};

	struct RenderPipelineLayoutInfo
	{
		struct Binding
		{
			UInt32 setIndex = 0;
			UInt32 bindingIndex;
			UInt32 arraySize = 1;
			ShaderBindingType type;
			nzsl::ShaderStageTypeFlags shaderStageFlags;
		};

		std::vector<Binding> bindings;
	};

	inline VkDescriptorType ToVulkan(ShaderBindingType bindingType)
	{
		switch (bindingType)
		{
			case ShaderBindingType::Sampler:
				return VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
			case ShaderBindingType::StorageBuffer:
				return VK_DESCRIPTOR_TYPE_STORAGE_BUFFER;
			case ShaderBindingType::Texture:
				return VK_DESCRIPTOR_TYPE_STORAGE_IMAGE;
			case ShaderBindingType::UniformBuffer:
				return VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
		}
		return VK_DESCRIPTOR_TYPE_SAMPLER;
	}

	inline VkShaderStageFlags ToVulkan(nzsl::ShaderStageTypeFlags stageType)
	{
		VkShaderStageFlags shaderStageBits = 0;
		for (nzsl::ShaderStageType shaderStage : stageType)
			shaderStageBits |= ToVulkan(shaderStage);

		return shaderStageBits;
	}
	
	class CONCERTO_PUBLIC_API ShaderReflection : public nzsl::Ast::RecursiveVisitor
	{
	public:
		ShaderReflection() = default;
		~ShaderReflection() = default;

		void Reflect(nzsl::Ast::Module& module);

		PipelineLayout BuildPipelineLayout(Device& device) const;
		
		struct ExternalData
		{
			UInt32 bindingSet;
			UInt32 bindingIndex;
		};
		
		struct ExternalUniformBlock : ExternalData
		{
			std::size_t structIndex;
		};

		struct ExternalStorageBlock : ExternalData
		{
			std::size_t structIndex;
		};
		
		struct ExternalSampler : ExternalData
		{
			UInt32 arraySize;
			nzsl::ImageType imageType;
			nzsl::Ast::PrimitiveType sampledType;
		};

		struct ExternalTexture : ExternalData
		{
			UInt32 arraySize;
			nzsl::AccessPolicy accessPolicy;
			nzsl::ImageFormat imageFormat;
			nzsl::ImageType imageType;
			nzsl::Ast::PrimitiveType baseType;
		};
		
		struct ExternalBlockData
		{
			std::unordered_map<std::string /*tag*/, ExternalSampler> samplers;
			std::unordered_map<std::string /*tag*/, ExternalStorageBlock> storageBlocks;
			std::unordered_map<std::string /*tag*/, ExternalTexture> textures;
			std::unordered_map<std::string /*tag*/, ExternalUniformBlock> uniformBlocks;
		};
		
		struct StructMemberData
		{
			std::size_t offset;
			std::size_t size;
			nzsl::Ast::ExpressionType type;
		};

		struct OptionData
		{
			nzsl::Ast::ExpressionType type;
			UInt32 hash;
		};

		struct StructData
		{
			StructData(nzsl::StructLayout layout) : fieldOffsets(layout) {}

			std::unordered_map<std::string /*tag*/, StructMemberData> members;
			nzsl::FieldOffsets fieldOffsets;
		};

	private:
		void Visit(nzsl::Ast::ConditionalStatement& node) override;
		void Visit(nzsl::Ast::DeclareExternalStatement& node) override;
		void Visit(nzsl::Ast::DeclareOptionStatement& node) override;
		void Visit(nzsl::Ast::DeclareStructStatement& node) override;
	
		std::unordered_map<std::string /*tag*/, ExternalBlockData> _externalBlocks;
		std::unordered_map<std::string /*name*/, OptionData> _options;
		std::unordered_map<std::size_t /*structIndex*/, StructData> _structs;
		bool _isConditional;
		RenderPipelineLayoutInfo _pipelineLayoutInfo;
	};
}

#endif //CONCERTOGRAPHICS_INCLUDE_SHADERREFLECTION_HPP_