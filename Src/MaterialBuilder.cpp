//
// Created by arthur on 17/02/2023.
//

#include <iostream>

#include "Concerto/Graphics/MaterialBuilder.hpp"
#include "Concerto/Graphics/Vulkan/Wrapper/Device.hpp"
#include "Concerto/Graphics/Vulkan/DescriptorBuilder.hpp"
#include "Concerto/Graphics/Vulkan/Wrapper/Sampler.hpp"
#include "Concerto/Graphics/Vulkan/Wrapper/ShaderModule.hpp"
#include "Concerto/Graphics/Vulkan/Wrapper/RenderPass.hpp"
#include "Concerto/Graphics/ShaderModuleInfo.hpp"

namespace Concerto::Graphics
{
	MaterialBuilder::MaterialBuilder(Device& device) :
		_layoutCache(device),
		_allocator(device),
		_sampler(device, VK_FILTER_NEAREST),
		_device(device),
		_materialsCache(),
		_descriptorPool(device)
	{

	}

	VkMaterialPtr MaterialBuilder::BuildMaterial(const MaterialInfo& material, RenderPass& renderpass)
	{
		auto& vertShaderModuleInfo = _shaderModuleInfos.emplace(material.vertexShaderPath, new ShaderModuleInfo(_device, material.vertexShaderPath)).first->second;
		auto& fragShaderModuleInfo = _shaderModuleInfos.emplace(material.fragmentShaderPath, new ShaderModuleInfo(_device, material.fragmentShaderPath)).first->second;
		
		std::vector<VkPipelineShaderStageCreateInfo> shaderStages = {
			VulkanInitializer::PipelineShaderStageCreateInfo(VK_SHADER_STAGE_VERTEX_BIT, *vertShaderModuleInfo->shaderModule->Get()),
			VulkanInitializer::PipelineShaderStageCreateInfo(VK_SHADER_STAGE_FRAGMENT_BIT, *fragShaderModuleInfo->shaderModule->Get())
		};
		std::vector<DescriptorSetLayoutPtr> descriptorSetLayouts;
		descriptorSetLayouts.reserve(vertShaderModuleInfo->pipelineLayout->GetDescriptorSetLayouts().size() + fragShaderModuleInfo->pipelineLayout->GetDescriptorSetLayouts().size());
		for (auto& descriptorSetLayout : vertShaderModuleInfo->pipelineLayout->GetDescriptorSetLayouts())
			descriptorSetLayouts.emplace_back(descriptorSetLayout);
		for (auto& descriptorSetLayout : fragShaderModuleInfo->pipelineLayout->GetDescriptorSetLayouts())
			descriptorSetLayouts.emplace_back(descriptorSetLayout);

		PipelineLayout pl(_device, descriptorSetLayouts);

		PipelineInfo pipelineInfo(std::move(shaderStages), renderpass.GetWindowExtent(), pl);
		
		auto pipeline = std::make_shared<Pipeline>(_device, pipelineInfo);
		pipeline->BuildPipeline(*renderpass.Get());
		auto it = _materialsCache.find(material);
		if (it != _materialsCache.end())
			return it->second;
		
		const auto& vertDescriptorSetLayouts = vertShaderModuleInfo->pipelineLayout->GetDescriptorSetLayouts();
		const auto& fragDescriptorSetLayouts = fragShaderModuleInfo->pipelineLayout->GetDescriptorSetLayouts();
		auto vkMaterialPtr = std::make_shared<VkMaterial>();
		vkMaterialPtr->_descriptorSets.reserve(vertDescriptorSetLayouts.size() + fragDescriptorSetLayouts.size());
		
		for (auto& descriptorSetLayout : vertDescriptorSetLayouts)
		{
			DescriptoSetInfoPtr descriptorSetInfo = std::make_shared<DescriptoSetInfo>(DescriptorSet(_device, _descriptorPool, *descriptorSetLayout));
			vkMaterialPtr->_descriptorSets.push_back(std::move(descriptorSetInfo));
		}

		for (auto& descriptorSetLayout : fragDescriptorSetLayouts)
		{
			DescriptoSetInfoPtr descriptorSetInfo = std::make_shared<DescriptoSetInfo>(DescriptorSet(_device, _descriptorPool, *descriptorSetLayout));
			vkMaterialPtr->_descriptorSets.push_back(std::move(descriptorSetInfo));
		}
		
		vkMaterialPtr->_pipeline = pipeline;
		vkMaterialPtr->_pipelineLayout = vertShaderModuleInfo->pipelineLayout.get();
		vkMaterialPtr->_diffuseTextureSet = nullptr;
		vkMaterialPtr->_normalTextureSet = nullptr;
		_materialsCache[material] = vkMaterialPtr;
		if (material.diffuseTexture == nullptr)
			return vkMaterialPtr;
		//DescriptorBuilder descriptorBuilder(_layoutCache, _allocator);
		//VkDescriptorImageInfo imageInfo{};
		//imageInfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
		//imageInfo.imageView = *material.diffuseTexture->_imageView.Get();
		//imageInfo.sampler = *_sampler.Get();
		//descriptorBuilder.BindImage(0, &imageInfo, VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, VK_SHADER_STAGE_FRAGMENT_BIT);
		//if (!descriptorBuilder.Build(vkMaterialPtr->_diffuseTextureSet))
		//{
		//	std::cerr << "Failed to build diffuse texture descriptor set" << std::endl;
		//}
		//vkMaterialPtr->_diffuseTextureSet->WriteImageSamplerDescriptor(_sampler, material.diffuseTexture->_imageView);
		//return vkMaterialPtr;
		return nullptr;
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