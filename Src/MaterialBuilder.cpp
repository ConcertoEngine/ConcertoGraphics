//
// Created by arthur on 17/02/2023.
//

#include "Concerto/Graphics/MaterialBuilder.hpp"

#include <ranges>

#include "Concerto/Graphics/TextureBuilder.hpp"
#include "Concerto/Graphics/Vulkan/Wrapper/Device.hpp"
#include "Concerto/Graphics/Vulkan/Wrapper/Sampler.hpp"
#include "Concerto/Graphics/Vulkan/Wrapper/ShaderModule.hpp"
#include "Concerto/Graphics/Vulkan/Wrapper/RenderPass.hpp"

namespace Concerto::Graphics
{
	MaterialBuilder::MaterialBuilder(Device& device) :
		_allocator(device),
		_sampler(device, VK_FILTER_NEAREST),
		_device(device),
		_materialsCache(),
		_descriptorPool(device)
	{

	}

	VkMaterialPtr MaterialBuilder::BuildMaterial(MaterialInfo& material, RenderPass& renderpass)
	{
		/*auto it = std::ranges::find_if(_materialsCache, [&](const auto& pair)
		{
			return pair.first.vertexShaderPath == material.vertexShaderPath && pair.first.fragmentShaderPath == material.fragmentShaderPath;
		});
		if (it != _materialsCache.end())
			return it->second;*/
		auto& vertShaderModuleInfo = _shaderModuleInfos.emplace(material.vertexShaderPath, new ShaderModuleInfo(_device, material.vertexShaderPath)).first->second;
		auto& fragShaderModuleInfo = _shaderModuleInfos.emplace(material.fragmentShaderPath, new ShaderModuleInfo(_device, material.fragmentShaderPath)).first->second;
		
		std::vector shaderStages = {
			VulkanInitializer::PipelineShaderStageCreateInfo(VK_SHADER_STAGE_VERTEX_BIT, *vertShaderModuleInfo->shaderModule->Get()),
			VulkanInitializer::PipelineShaderStageCreateInfo(VK_SHADER_STAGE_FRAGMENT_BIT, *fragShaderModuleInfo->shaderModule->Get())
		};

		std::unordered_map<UInt32 /*set*/, std::vector<VkDescriptorSetLayoutBinding>> bindings = vertShaderModuleInfo->bindings;
		for (auto& [set, setBindings] : fragShaderModuleInfo->bindings)
		{
			auto it = bindings.find(set);
			if (it == bindings.end())
				bindings.emplace(set, setBindings);
			else
				it->second.insert(it->second.end(), setBindings.begin(), setBindings.end());
		}

		std::vector<DescriptorSetLayoutPtr> descriptorSetLayouts;
		for (auto& setBindings : bindings | std::views::values)
			descriptorSetLayouts.push_back(GeDescriptorSetLayout(setBindings));

		auto pl = std::make_shared<PipelineLayout>(_device, descriptorSetLayouts);

		PipelineInfo pipelineInfo(std::move(shaderStages), renderpass.GetWindowExtent(), pl);
		
		auto pipeline = std::make_shared<Pipeline>(_device, pipelineInfo);
		pipeline->BuildPipeline(*renderpass.Get());

		auto vkMaterialPtr = std::make_shared<VkMaterial>();
		_materialsCache.emplace(vkMaterialPtr);
		vkMaterialPtr->descriptorSets.reserve(descriptorSetLayouts.size());
		
		for (auto& descriptorSetLayout : descriptorSetLayouts)
		{
			auto& descriptorSet = vkMaterialPtr->descriptorSets.emplace_back();
			if (!_allocator.Allocate(descriptorSet, *descriptorSetLayout))
			{
				CONCERTO_ASSERT_FALSE;
			}
		}

		vkMaterialPtr->pipeline = pipeline;
		vkMaterialPtr->pipelineLayout = pipeline->GetPipelineLayout();
		vkMaterialPtr->diffuseTextureSet = nullptr;
		vkMaterialPtr->normalTextureSet = nullptr;

		if (material.diffuseTexturePath.empty())
			return vkMaterialPtr;
		material.diffuseTexture = TextureBuilder::Instance()->BuildTexture(material.diffuseTexturePath);
		VkDescriptorImageInfo imageInfo = {};
		imageInfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
		imageInfo.imageView = *material.diffuseTexture->_imageView.Get();
		imageInfo.sampler = *_sampler.Get();

		VkWriteDescriptorSet newWrite = {};
		newWrite.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
		newWrite.pNext = nullptr;
		newWrite.descriptorCount = 1;
		newWrite.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
		newWrite.pImageInfo = &imageInfo;
		newWrite.dstBinding = 0;
		newWrite.dstSet = *vkMaterialPtr->descriptorSets[2]->Get();

		_device.UpdateDescriptorSetWrite(newWrite);
		vkMaterialPtr->descriptorSets[2]->WriteImageSamplerDescriptor(_sampler, material.diffuseTexture->_imageView);
		return vkMaterialPtr;
	}

	VkMaterialPtr MaterialBuilder::GetMaterial(const std::string& materialName)
	{
		/*for (auto& [material, vkMaterial] : _materialsCache)
		{
			if (material.name == materialName)
				return vkMaterial;
		}*/
		return nullptr;
	}

	std::set<VkMaterialPtr> MaterialBuilder::GetMaterials()
	{
		/*std::set<VkMaterial*> materials;
		auto toto = this->_materialsCache.size();
		for (auto& [materialInfo, vkMaterial] : _materialsCache)
			materials.emplace(vkMaterial.get());
		return materials;*/
		return _materialsCache;
	}

	DescriptorSetLayoutPtr MaterialBuilder::GeDescriptorSetLayout(const std::vector<VkDescriptorSetLayoutBinding>& bindings)
	{
		UInt64 hash = DescriptorSetLayout::GetHash(bindings);

		const auto it = _descriptorSetLayoutsCache.find(hash);
		if (it != _descriptorSetLayoutsCache.end())
			return it->second;

		DescriptorSetLayoutPtr descriptorSetLayout = MakeDescriptorSetLayout(_device, bindings);
		_descriptorSetLayoutsCache.emplace(hash, descriptorSetLayout);
		return descriptorSetLayout;
	}
}
