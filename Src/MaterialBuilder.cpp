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
		_device(device),
		_materialsCache(),
		_descriptorPool(device),
		_sampler(device, VK_FILTER_NEAREST)
	{

	}

	VkMaterialPtr MaterialBuilder::BuildMaterial(MaterialInfo& material, RenderPass& renderPass)
	{
		ShaderModuleInfo* vertexShaderModuleInfo = nullptr;
		ShaderModuleInfo* fragShaderModuleInfo = nullptr;
		{
			auto it = _shaderModuleInfos.find(material.vertexShaderPath);
			if (it == _shaderModuleInfos.end())
				vertexShaderModuleInfo = &_shaderModuleInfos.emplace(material.vertexShaderPath, ShaderModuleInfo(_device, material.vertexShaderPath)).first->second;
			else vertexShaderModuleInfo = &it->second;
			it = _shaderModuleInfos.find(material.fragmentShaderPath);
			if (it == _shaderModuleInfos.end())
				fragShaderModuleInfo = &_shaderModuleInfos.emplace(material.fragmentShaderPath, ShaderModuleInfo(_device, material.fragmentShaderPath)).first->second;
			else fragShaderModuleInfo = &it->second;
		}
		
		std::unordered_map<UInt32 /*set*/, std::vector<VkDescriptorSetLayoutBinding>> bindings = vertexShaderModuleInfo->bindings;
		for (auto& [set, setBindings] : fragShaderModuleInfo->bindings)
		{
			auto it = bindings.find(set);
			if (it == bindings.end())
				bindings.emplace(set, setBindings);
			else
				it->second.insert(it->second.end(), setBindings.begin(), setBindings.end());
		}

		std::vector<DescriptorSetLayoutPtr> descriptorSetLayouts;
		DescriptorSetLayoutPtr texture = nullptr;
		DescriptorSetPtr textureDescriptorSet = nullptr;
		UInt32 destinationBinding = 0;
		for (auto& setBindings : bindings | std::views::values)
		{
			if (setBindings.size() == 1 && setBindings[0].descriptorType == VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER)
			{
				if (!texture)
				{
					for (auto binding : setBindings)
					{
						if (binding.descriptorType == VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER)
							destinationBinding = binding.binding;
					}
					texture = MakeDescriptorSetLayout(_device, setBindings);
					descriptorSetLayouts.push_back(texture);
				}
				else CONCERTO_ASSERT_FALSE;
				continue;
			}
			descriptorSetLayouts.push_back(GeDescriptorSetLayout(setBindings));
		}

		auto vkMaterialPtr = std::make_shared<VkMaterial>();
		_materialsCache.emplace(vkMaterialPtr);
		vkMaterialPtr->descriptorSets.reserve(descriptorSetLayouts.size());

		PipelinePtr pipeline;
		UInt64 hash = 0;
		std::hash<std::string> hasher;
		hash ^= hasher(material.vertexShaderPath) + 0x9e3779b9 + (hash << 6) + (hash >> 2);
		hash ^= hasher(material.fragmentShaderPath) + 0x9e3779b9 + (hash << 6) + (hash >> 2);
		{
			auto it = _pipelinesCache.find(hash);
			if (it != _pipelinesCache.end())
				pipeline = it->second;
			else
			{
				std::vector shaderStages = {
					VulkanInitializer::PipelineShaderStageCreateInfo(VK_SHADER_STAGE_VERTEX_BIT, *vertexShaderModuleInfo->shaderModule->Get()),
					VulkanInitializer::PipelineShaderStageCreateInfo(VK_SHADER_STAGE_FRAGMENT_BIT, *fragShaderModuleInfo->shaderModule->Get())
				};
				auto pl = std::make_shared<PipelineLayout>(_device, descriptorSetLayouts);
				PipelineInfo pipelineInfo(std::move(shaderStages), renderPass.GetWindowExtent(), pl);
				pipeline = std::make_shared<Pipeline>(_device, std::move(pipelineInfo));
				pipeline->BuildPipeline(*renderPass.Get());
				_pipelinesCache.emplace(hash, pipeline);
			}
		}
		vkMaterialPtr->pipeline = pipeline;

		for (auto& descriptorSetLayout : descriptorSetLayouts)
		{
			auto& descriptorSet = vkMaterialPtr->descriptorSets.emplace_back();
			bool needNewDescriptor = false;
			for (auto& binding : descriptorSetLayout->GetBindings())
			{
				if (binding.descriptorType == VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER)
					needNewDescriptor = true;
			}
			if (needNewDescriptor)
			{
				if (!_allocator.AllocateWithoutCache(descriptorSet, *descriptorSetLayout))
					CONCERTO_ASSERT_FALSE;
				textureDescriptorSet = descriptorSet;
			}
			else
			{
				if (!_allocator.Allocate(descriptorSet, *descriptorSetLayout))
					CONCERTO_ASSERT_FALSE;
			}
		}

		if (!material.diffuseTexturePath.empty())
		{
			vkMaterialPtr->diffuseTexture = TextureBuilder::Instance()->BuildTexture(material.diffuseTexturePath);
			VkDescriptorImageInfo imageInfo = {};
			imageInfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
			imageInfo.imageView = *vkMaterialPtr->diffuseTexture->_imageView.Get();
			imageInfo.sampler = *_sampler.Get();

			VkWriteDescriptorSet newWrite = {};
			newWrite.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
			newWrite.pNext = nullptr;
			newWrite.descriptorCount = 1;
			newWrite.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
			newWrite.pImageInfo = &imageInfo;
			newWrite.dstBinding = destinationBinding;
			newWrite.dstSet = *textureDescriptorSet->Get();

			_device.UpdateDescriptorSetWrite(newWrite);
			textureDescriptorSet->WriteImageSamplerDescriptor(_sampler, vkMaterialPtr->diffuseTexture->_imageView);
		}
		return vkMaterialPtr;
	}

	std::vector<VkPipelineShaderStageCreateInfo> MaterialBuilder::GetShaderStages() const
	{
		std::vector<VkPipelineShaderStageCreateInfo> shaderStages;
		for (const auto& shaderModuleInfo : _shaderModuleInfos | std::views::values)
			shaderStages.push_back(shaderModuleInfo.shaderModule->GetPipelineShaderStageCreateInfo());
		return shaderStages;
	}

	std::vector<DescriptorSetLayoutPtr> MaterialBuilder::GetDescriptorSetLayouts() const
	{
		std::vector<DescriptorSetLayoutPtr> descriptorSetLayouts;

		for (const auto& layout : _descriptorSetLayoutsCache | std::views::values)
			descriptorSetLayouts.push_back(layout);
		return descriptorSetLayouts;
	}

	std::set<VkMaterialPtr> MaterialBuilder::GetMaterials()
	{
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
