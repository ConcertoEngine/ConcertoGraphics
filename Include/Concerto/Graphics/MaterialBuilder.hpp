//
// Created by arthur on 17/02/2023.
//

#ifndef CONCERTO_GRAPHICS_INCLUDE_MATERIALBUILDER_HPP_
#define CONCERTO_GRAPHICS_INCLUDE_MATERIALBUILDER_HPP_

#include <set>
#include <string>
#include <unordered_map>

#include "Concerto/Graphics/Material.hpp"
#include "Concerto/Graphics/Vulkan/VkMaterial.hpp"
#include "Concerto/Graphics/Vulkan/Wrapper/Sampler.hpp"
#include "Concerto/Graphics/Vulkan/DescriptorAllocator.hpp"
#include "Concerto/Graphics/ShaderModuleInfo.hpp"

namespace Concerto::Graphics
{
	class RenderPass;
	class Pipeline;
	class DescriptorLayoutCache;
	
	class CONCERTO_GRAPHICS_API MaterialBuilder
	{
	 public:
		explicit MaterialBuilder(Device& device);
		VkMaterialPtr BuildMaterial(MaterialInfo& material, RenderPass& renderPass);

		[[nodiscard]] std::vector<VkPipelineShaderStageCreateInfo> GetShaderStages() const;
		[[nodiscard]] std::vector<std::shared_ptr<DescriptorSetLayout>> GetDescriptorSetLayouts() const;
		std::set<VkMaterialPtr> GetMaterials();
	 private:
		std::shared_ptr<DescriptorSetLayout> GeDescriptorSetLayout(const std::vector<VkDescriptorSetLayoutBinding>& bindings);
		
		DescriptorAllocator _allocator;
		Device& _device;
		std::set<VkMaterialPtr> _materialsCache;
		std::unordered_map<UInt64 /*hash*/, std::shared_ptr<Pipeline>> _pipelinesCache;
		std::unordered_map<std::string, ShaderModuleInfo> _shaderModuleInfos;
		DescriptorPool _descriptorPool;
		std::unordered_map<UInt64 /*hash*/, std::shared_ptr<DescriptorSetLayout>> _descriptorSetLayoutsCache;
		std::vector<Pipeline> _pipelines;
		Sampler _sampler;
	};
}
#endif //CONCERTO_GRAPHICS_INCLUDE_MATERIALBUILDER_HPP_
