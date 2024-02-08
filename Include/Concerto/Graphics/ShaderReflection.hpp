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