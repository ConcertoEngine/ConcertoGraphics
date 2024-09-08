//
// Created by arthur on 17/02/2023.
//

#include "Concerto/Graphics/RHI/Vulkan/VkRHIMaterialBuilder.hpp"

#include <ranges>

#include "Concerto/Core/Cast.hpp"

#include "Concerto/Graphics/Backend/Vulkan/Wrapper/DescriptorSetLayout.hpp"
#include "Concerto/Graphics/Backend/Vulkan/Wrapper/Pipeline.hpp"
#include "Concerto/Graphics/Backend/Vulkan/Wrapper/PipelineInfo.hpp"
#include "Concerto/Graphics/Backend/Vulkan/Wrapper/VulkanInitializer.hpp"
#include "Concerto/Graphics/Backend/Vulkan/Wrapper/Device.hpp"
#include "Concerto/Graphics/Backend/Vulkan/Wrapper/Sampler.hpp"
#include "Concerto/Graphics/Backend/Vulkan/Wrapper/ShaderModule.hpp"
#include "Concerto/Graphics/RHI/Vulkan/VkRHIBuffer.hpp"

#include "Concerto/Graphics/RHI/Vulkan/VkRHITextureBuilder.hpp"
#include "Concerto/Graphics/RHI/Vulkan/VKRHIRenderPass.hpp"
#include "Concerto/Graphics/RHI/Vulkan/VKRHITexture.hpp"

namespace Concerto::Graphics::RHI
{
	VkRHIMaterialBuilder::VkRHIMaterialBuilder(Vk::Device& device, VkExtent2D windowExtent) :
		_allocator(device),
		_device(device),
		_materialsCache(),
		_descriptorPool(device),
		_sampler(device, VK_FILTER_NEAREST),
		_windowExtent(windowExtent)
	{

	}

	MaterialPtr VkRHIMaterialBuilder::BuildMaterial(RHI::MaterialInfo& material, const RHI::RenderPass& renderPass)
	{
		Vk::ShaderModuleInfo* vertexShaderModuleInfo = nullptr;
		Vk::ShaderModuleInfo* fragShaderModuleInfo = nullptr;
		{
			auto it = _shaderModuleInfos.find(material.vertexShaderPath);
			if (it == _shaderModuleInfos.end())
				vertexShaderModuleInfo = &_shaderModuleInfos.emplace(material.vertexShaderPath, Vk::ShaderModuleInfo(_device, material.vertexShaderPath)).first->second;
			else vertexShaderModuleInfo = &it->second;
			it = _shaderModuleInfos.find(material.fragmentShaderPath);
			if (it == _shaderModuleInfos.end())
				fragShaderModuleInfo = &_shaderModuleInfos.emplace(material.fragmentShaderPath, Vk::ShaderModuleInfo(_device, material.fragmentShaderPath)).first->second;
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

		std::vector<std::shared_ptr<Vk::DescriptorSetLayout>> descriptorSetLayouts;
		std::shared_ptr<Vk::DescriptorSetLayout> texture = nullptr;
		Vk::DescriptorSetPtr textureDescriptorSet = nullptr;
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
					texture = Vk::MakeDescriptorSetLayout(_device, setBindings);
					descriptorSetLayouts.push_back(texture);
				}
				else
				{
					CONCERTO_ASSERT_FALSE("ConcertoGraphics: Multiple textures are not supported yet");
				}
				continue;
			}
			descriptorSetLayouts.push_back(GeDescriptorSetLayout(setBindings));
		}

		auto vkMaterialPtr = std::make_shared<Vk::VkMaterial>();
		_materialsCache.emplace(vkMaterialPtr);
		vkMaterialPtr->descriptorSets.reserve(descriptorSetLayouts.size());

		Vk::PipelinePtr pipeline;
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
				auto pl = std::make_shared<Vk::PipelineLayout>(_device, descriptorSetLayouts);
				const VkRHIRenderPass& vkRenderPass = Cast<const VkRHIRenderPass&>(renderPass);
				Vk::PipelineInfo pipelineInfo(std::move(shaderStages), _windowExtent, pl);
				pipeline = std::make_shared<Vk::Pipeline>(_device, std::move(pipelineInfo));
				pipeline->BuildPipeline(*vkRenderPass.Get());
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
				{
					CONCERTO_ASSERT_FALSE("ConcertoGraphics: Cannot allocate descriptor set");
				}
				textureDescriptorSet = descriptorSet;
			}
			else
			{
				if (!_allocator.Allocate(descriptorSet, *descriptorSetLayout))
				{
					CONCERTO_ASSERT_FALSE("ConcertoGraphics: Cannot allocate descriptor set");
				}
			}
		}

		if (!material.diffuseTexturePath.empty())
		{
			vkMaterialPtr->diffuseTexture = RHI::VkRHITextureBuilder::Instance()->BuildTexture(material.diffuseTexturePath);
			auto& vkTexture = Cast<VkRHITexture&>(*vkMaterialPtr->diffuseTexture);
			VkDescriptorImageInfo imageInfo = {};
			imageInfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
			imageInfo.imageView = *vkTexture.GetImageView().Get();
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
			textureDescriptorSet->WriteImageSamplerDescriptor(_sampler, vkTexture.GetImageView());
		}
		return vkMaterialPtr;
	}

	void VkRHIMaterialBuilder::Update(const RHI::Buffer& buffer, UInt32 setIndex, UInt32 bindingIndex)
	{
		const VkRHIBuffer& vkBuffer = Cast<const VkRHIBuffer&>(buffer);
		VkDescriptorBufferInfo bufferInfo;
		bufferInfo.buffer = *vkBuffer.Get();
		bufferInfo.offset = 0;
		bufferInfo.range = vkBuffer.GetAllocatedSize();
		const VkDescriptorType descriptorType = [&]() -> VkDescriptorType
		{
			const auto usageFlags = vkBuffer.GetUsage();
			if (usageFlags & VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT)
				return VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
			if (usageFlags & VK_BUFFER_USAGE_STORAGE_BUFFER_BIT)
				return VK_DESCRIPTOR_TYPE_STORAGE_BUFFER;
			if (usageFlags & VK_BUFFER_USAGE_UNIFORM_TEXEL_BUFFER_BIT)
				return VK_DESCRIPTOR_TYPE_UNIFORM_TEXEL_BUFFER;
			if (usageFlags & VK_BUFFER_USAGE_STORAGE_TEXEL_BUFFER_BIT)
				return VK_DESCRIPTOR_TYPE_STORAGE_TEXEL_BUFFER;
			//CONCERTO_ASSERT("ConcertoGraphics: enum value not handled");
			return {};
		}();
		for (const auto& material : _materialsCache)
		{
			const VkWriteDescriptorSet cameraWrite = VulkanInitializer::WriteDescriptorBuffer(
				descriptorType, *material->descriptorSets[setIndex]->Get(), &bufferInfo, bindingIndex);
			_device.UpdateDescriptorSetWrite(cameraWrite);
		}
	}

	std::vector<VkPipelineShaderStageCreateInfo> VkRHIMaterialBuilder::GetShaderStages() const
	{
		std::vector<VkPipelineShaderStageCreateInfo> shaderStages;
		for (const auto& shaderModuleInfo : _shaderModuleInfos | std::views::values)
			shaderStages.push_back(shaderModuleInfo.shaderModule->GetPipelineShaderStageCreateInfo());
		return shaderStages;
	}

	std::vector<std::shared_ptr<Vk::DescriptorSetLayout>> VkRHIMaterialBuilder::GetDescriptorSetLayouts() const
	{
		std::vector<std::shared_ptr<Vk::DescriptorSetLayout>> descriptorSetLayouts;

		for (const auto& layout : _descriptorSetLayoutsCache | std::views::values)
			descriptorSetLayouts.push_back(layout);
		return descriptorSetLayouts;
	}

	std::set<Vk::VkMaterialPtr> VkRHIMaterialBuilder::GetMaterials()
	{
		return _materialsCache;
	}

	std::shared_ptr<Vk::DescriptorSetLayout> VkRHIMaterialBuilder::GeDescriptorSetLayout(const std::vector<VkDescriptorSetLayoutBinding>& bindings)
	{
		UInt64 hash = Vk::DescriptorSetLayout::GetHash(bindings);

		const auto it = _descriptorSetLayoutsCache.find(hash);
		if (it != _descriptorSetLayoutsCache.end())
			return it->second;

		std::shared_ptr<Vk::DescriptorSetLayout> descriptorSetLayout = MakeDescriptorSetLayout(_device, bindings);
		_descriptorSetLayoutsCache.emplace(hash, descriptorSetLayout);
		return descriptorSetLayout;
	}
}
