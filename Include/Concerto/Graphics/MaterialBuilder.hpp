//
// Created by arthur on 17/02/2023.
//

#ifndef CONCERTOGRAPHICS_INCLUDE_MATERIALBUILDER_HPP_
#define CONCERTOGRAPHICS_INCLUDE_MATERIALBUILDER_HPP_

#include <string>
#include <unordered_map>

#include "Concerto/Graphics/Material.hpp"
#include "Concerto/Graphics/Vulkan/VkMaterial.hpp"

namespace Concerto::Graphics
{
	class DescriptorLayoutCache;
	class DescriptorAllocator;
	class Sampler;

	class MaterialBuilder
	{
	 public:
		MaterialBuilder(DescriptorLayoutCache& layoutCache, DescriptorAllocator& allocator, Sampler &sampler);
		VkMaterialPtr BuildMaterial(const Material& material, VkPipelineLayout pipelineLayout, VkPipeline pipeline);
		VkMaterialPtr GetMaterial(const std::string& materialName);
	 private:
		std::unordered_map<Material, VkMaterialPtr, Material::Hash> _materialsCache;
		DescriptorLayoutCache& _layoutCache;
		DescriptorAllocator& _allocator;
		Sampler& _sampler;
	};
}
#endif //CONCERTOGRAPHICS_INCLUDE_MATERIALBUILDER_HPP_
