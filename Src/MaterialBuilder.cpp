//
// Created by arthur on 17/02/2023.
//

#include <iostream>

#include "Concerto/Graphics/MaterialBuilder.hpp"
#include "Concerto/Graphics/Vulkan/DescriptorBuilder.hpp"
#include "Concerto/Graphics/Vulkan/Wrapper/Sampler.hpp"

namespace Concerto::Graphics
{
	MaterialBuilder::MaterialBuilder(DescriptorLayoutCache& layoutCache,
		DescriptorAllocator& allocator,
		Sampler& sampler) :
		_layoutCache(layoutCache), _allocator(allocator), _sampler(sampler)
	{

	}

	VkMaterialPtr MaterialBuilder::BuildMaterial(const Material& material,
		VkPipelineLayout pipelineLayout,
		VkPipeline pipeline)
	{
		auto it = _materialsCache.find(material);
		if (it != _materialsCache.end())
			return it->second;
		auto vkMaterial = std::make_shared<VkMaterial>();
		vkMaterial->_pipelineLayout = pipelineLayout;
		vkMaterial->_pipeline = pipeline;
		_materialsCache[material] = vkMaterial;
		if (material.diffuseTexture == nullptr)
			return vkMaterial;
		DescriptorBuilder descriptorBuilder(_layoutCache, _allocator);
		VkDescriptorImageInfo imageInfo{};
		imageInfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
		imageInfo.imageView = *material.diffuseTexture->_imageView.Get();
		imageInfo.sampler = *_sampler.Get();
		descriptorBuilder.BindImage(0,
			&imageInfo,
			VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER,
			VK_SHADER_STAGE_FRAGMENT_BIT);
		if (!descriptorBuilder.Build(vkMaterial->_diffuseTextureSet))
		{
			std::cerr << "Failed to build diffuse texture descriptor set" << std::endl;
		}
		vkMaterial->_diffuseTextureSet->WriteImageSamplerDescriptor(_sampler, material.diffuseTexture->_imageView);
		return vkMaterial;
	}

	VkMaterialPtr MaterialBuilder::GetMaterial(const std::string& materialName)
	{
		for (auto& [material, vkMaterial] : _materialsCache)
		{
			if (material.name == materialName)
				return vkMaterial;
		}
		return nullptr;
	}
}