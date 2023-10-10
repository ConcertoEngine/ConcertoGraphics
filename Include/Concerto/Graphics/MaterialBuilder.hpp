//
// Created by arthur on 17/02/2023.
//

#ifndef CONCERTOGRAPHICS_INCLUDE_MATERIALBUILDER_HPP_
#define CONCERTOGRAPHICS_INCLUDE_MATERIALBUILDER_HPP_

#include <string>
#include <unordered_map>

#include "Concerto/Graphics/Material.hpp"
#include "Concerto/Graphics/Vulkan/VkMaterial.hpp"
#include "Concerto/Graphics/Vulkan/Wrapper/Sampler.hpp"
#include "Concerto/Graphics/Vulkan/DescriptorAllocator.hpp"
#include "Concerto/Graphics/Vulkan/DescriptorLayoutCache.hpp"
#include "Concerto/Graphics/ShaderModuleInfo.hpp"
#include <NZSL/Parser.hpp>
#include <NZSL/SpirvWriter.hpp>
#include <NZSL/Ast/ReflectVisitor.hpp>
#include <NZSL/Ast/SanitizeVisitor.hpp>

namespace Concerto::Graphics
{
	class RenderPass;
	
	class CONCERTO_GRAPHICS_API MaterialBuilder
	{
	 public:
		MaterialBuilder(Device& device);
		VkMaterialPtr BuildMaterial(const MaterialInfo& material, RenderPass& renderpass);
		VkMaterialPtr GetMaterial(const std::string& materialName);
	 private:
		DescriptorLayoutCache _layoutCache;
		DescriptorAllocator _allocator;
		Sampler _sampler;
		Device& _device;
		std::unordered_map<MaterialInfo, VkMaterialPtr, MaterialInfo::Hash> _materialsCache;
		std::unordered_map<std::string, ShaderModuleInfo*> _shaderModuleInfos;
		DescriptorPool _descriptorPool;
	};
}
#endif //CONCERTOGRAPHICS_INCLUDE_MATERIALBUILDER_HPP_
