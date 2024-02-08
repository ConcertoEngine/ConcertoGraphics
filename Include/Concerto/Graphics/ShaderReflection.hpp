//
// Created by arthur on 23/08/2023.
//

#ifndef CONCERTO_GRAPHICS_INCLUDE_SHADERREFLECTION_HPP_
#define CONCERTO_GRAPHICS_INCLUDE_SHADERREFLECTION_HPP_

#include <unordered_map>
#include <vector>

#include <vulkan/vulkan.h>
#include <NZSL/Ast/RecursiveVisitor.hpp>
#include <NZSL/Math/FieldOffsets.hpp>
#include <NZSL/Ast/Module.hpp>

#include "Concerto/Graphics/Defines.hpp"
#include "Concerto/Graphics/Vulkan/Wrapper/DescriptorSetLayout.hpp"

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

	struct CONCERTO_GRAPHICS_API RenderPipelineLayoutInfo
	{
		struct CONCERTO_GRAPHICS_API Binding
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

	inline VkShaderStageFlags ToVulkan(nzsl::ShaderStageType stageType)
	{
		switch (stageType)
		{
		case nzsl::ShaderStageType::Vertex:
			return VK_SHADER_STAGE_VERTEX_BIT;
		case nzsl::ShaderStageType::Fragment:
			return VK_SHADER_STAGE_FRAGMENT_BIT;
		case nzsl::ShaderStageType::Compute:
			return VK_SHADER_STAGE_COMPUTE_BIT;
		}
		return VK_SHADER_STAGE_ALL;
	}

	inline VkShaderStageFlags ToVulkan(nzsl::ShaderStageTypeFlags stageType)
	{
		VkShaderStageFlags shaderStageBits = 0;
		for (nzsl::ShaderStageType shaderStage : stageType)
			shaderStageBits |= ToVulkan(shaderStage);

		return shaderStageBits;
	}
	
	class CONCERTO_GRAPHICS_API ShaderReflection : public nzsl::Ast::RecursiveVisitor
	{
	public:
		ShaderReflection() = default;
		~ShaderReflection() = default;

		void Reflect(nzsl::Ast::Module& module);

		std::unique_ptr<PipelineLayout> BuildPipelineLayout(Device& device);
		
		struct ExternalData
		{
			UInt32 bindingSet;
			UInt32 bindingIndex;
		};
		
		struct CONCERTO_GRAPHICS_API ExternalUniformBlock : ExternalData
		{
			std::size_t structIndex;
		};

		struct CONCERTO_GRAPHICS_API ExternalStorageBlock : ExternalData
		{
			std::size_t structIndex;
		};

		struct CONCERTO_GRAPHICS_API ExternalSampler : ExternalData
		{
			UInt32 arraySize;
			nzsl::ImageType imageType;
			nzsl::Ast::PrimitiveType sampledType;
		};

		struct CONCERTO_GRAPHICS_API ExternalTexture : ExternalData
		{
			UInt32 arraySize;
			nzsl::AccessPolicy accessPolicy;
			nzsl::ImageFormat imageFormat;
			nzsl::ImageType imageType;
			nzsl::Ast::PrimitiveType baseType;
		};

		struct CONCERTO_GRAPHICS_API ExternalBlockData
		{
			std::unordered_map<std::string /*tag*/, ExternalSampler> samplers;
			std::unordered_map<std::string /*tag*/, ExternalStorageBlock> storageBlocks;
			std::unordered_map<std::string /*tag*/, ExternalTexture> textures;
			std::unordered_map<std::string /*tag*/, ExternalUniformBlock> uniformBlocks;
		};

		struct CONCERTO_GRAPHICS_API StructMemberData
		{
			std::size_t offset;
			std::size_t size;
			nzsl::Ast::ExpressionType type;
		};

		struct CONCERTO_GRAPHICS_API OptionData
		{
			nzsl::Ast::ExpressionType type;
			UInt32 hash;
		};

		struct CONCERTO_GRAPHICS_API StructData
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
		struct VkDescriptorSetLayoutBindingHash
		{
			std::size_t operator()(const VkDescriptorSetLayoutBinding& binding) const
			{
				return std::hash<UInt32>()(binding.binding) ^
					   std::hash<UInt32>()(binding.descriptorCount) ^
					   std::hash<UInt32>()(UInt32(binding.descriptorType)) ^
					   std::hash<UInt32>()(UInt32(binding.stageFlags));
			}

		};
		std::unordered_map<VkDescriptorSetLayoutBinding, DescriptorSetLayoutPtr, VkDescriptorSetLayoutBindingHash> _descriptorSetLayoutsCache;
	};
}

inline bool operator==(const VkDescriptorSetLayoutBinding& lhs, const VkDescriptorSetLayoutBinding& rhs)
{
	return lhs.binding == rhs.binding &&
		lhs.descriptorCount == rhs.descriptorCount &&
		lhs.descriptorType == rhs.descriptorType &&
		lhs.stageFlags == rhs.stageFlags;
}

inline bool operator!=(const VkDescriptorSetLayoutBinding& lhs, const VkDescriptorSetLayoutBinding& rhs)
{
	return !(lhs == rhs);
}

#endif //CONCERTO_GRAPHICS_INCLUDE_SHADERREFLECTION_HPP_