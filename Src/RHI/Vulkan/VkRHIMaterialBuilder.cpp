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

namespace cct::gfx::rhi
{
	VkRHIMaterialBuilder::VkRHIMaterialBuilder(vk::Device& device, VkExtent2D windowExtent) :
		m_allocator(device),
		m_device(device),
		m_materialsCache(),
		m_descriptorPool(device),
		m_sampler(device, VK_FILTER_NEAREST),
		m_windowExtent(windowExtent)
	{

	}

	MaterialPtr VkRHIMaterialBuilder::BuildMaterial(rhi::MaterialInfo& material, const rhi::RenderPass& renderPass)
	{
		CCT_GFX_AUTO_PROFILER_SCOPE();

		vk::ShaderModuleInfo* vertexShaderModuleInfo = nullptr;
		vk::ShaderModuleInfo* fragShaderModuleInfo = nullptr;
		{
			CCT_GFX_PROFILER_SCOPE("Find or create ShaderModule");
			auto it = m_shaderModuleInfos.find(material.vertexShaderPath);
			if (it == m_shaderModuleInfos.end())
				vertexShaderModuleInfo = &m_shaderModuleInfos.emplace(material.vertexShaderPath, vk::ShaderModuleInfo(m_device, material.vertexShaderPath)).first->second;
			else vertexShaderModuleInfo = &it->second;
			it = m_shaderModuleInfos.find(material.fragmentShaderPath);
			if (it == m_shaderModuleInfos.end())
				fragShaderModuleInfo = &m_shaderModuleInfos.emplace(material.fragmentShaderPath, vk::ShaderModuleInfo(m_device, material.fragmentShaderPath)).first->second;
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

		std::vector<std::shared_ptr<vk::DescriptorSetLayout>> descriptorSetLayouts;
		std::shared_ptr<vk::DescriptorSetLayout> texture = nullptr;
		vk::DescriptorSetPtr textureDescriptorSet = nullptr;
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
					texture = vk::MakeDescriptorSetLayout(m_device, setBindings);
					descriptorSetLayouts.push_back(texture);
				}
				else
				{
					CCT_ASSERT_FALSE("ConcertoGraphics: Multiple textures are not supported yet");
				}
				continue;
			}
			descriptorSetLayouts.push_back(GeDescriptorSetLayout(setBindings));
		}

		auto vkMaterialPtr = std::make_shared<vk::VkMaterial>();
		*static_cast<MaterialInfo*>(vkMaterialPtr.get()) = material; // burk
		m_materialsCache.emplace(vkMaterialPtr);
		vkMaterialPtr->descriptorSets.reserve(descriptorSetLayouts.size());

		vk::PipelinePtr pipeline;
		UInt64 hash = 0;
		std::hash<std::string> hasher;
		hash ^= hasher(material.vertexShaderPath) + 0x9e3779b9 + (hash << 6) + (hash >> 2);
		hash ^= hasher(material.fragmentShaderPath) + 0x9e3779b9 + (hash << 6) + (hash >> 2);
		{
			auto it = m_pipelinesCache.find(hash);
			if (it != m_pipelinesCache.end())
				pipeline = it->second;
			else
			{
				std::vector shaderStages = {
					VulkanInitializer::PipelineShaderStageCreateInfo(VK_SHADER_STAGE_VERTEX_BIT, *vertexShaderModuleInfo->shaderModule->Get()),
					VulkanInitializer::PipelineShaderStageCreateInfo(VK_SHADER_STAGE_FRAGMENT_BIT, *fragShaderModuleInfo->shaderModule->Get())
				};
				auto pl = std::make_shared<vk::PipelineLayout>(m_device, descriptorSetLayouts);
				const VkRHIRenderPass& vkRenderPass = Cast<const VkRHIRenderPass&>(renderPass);
				vk::PipelineInfo pipelineInfo(std::move(shaderStages), m_windowExtent, pl);
				pipeline = std::make_shared<vk::Pipeline>(m_device, std::move(pipelineInfo));
				pipeline->BuildPipeline(*vkRenderPass.Get());
				m_pipelinesCache.emplace(hash, pipeline);
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
				if (!m_allocator.AllocateWithoutCache(descriptorSet, *descriptorSetLayout))
				{
					CCT_ASSERT_FALSE("ConcertoGraphics: Cannot allocate descriptor set");
				}
				textureDescriptorSet = descriptorSet;
			}
			else
			{
				if (!m_allocator.Allocate(descriptorSet, *descriptorSetLayout))
				{
					CCT_ASSERT_FALSE("ConcertoGraphics: Cannot allocate descriptor set");
				}
			}
		}

		if (!material.diffuseTexturePath.empty())
		{
			vkMaterialPtr->diffuseTexture = rhi::VkRHITextureBuilder::Instance()->BuildTexture(material.diffuseTexturePath);
			auto& vkTexture = Cast<VkRHITexture&>(*vkMaterialPtr->diffuseTexture);
			VkDescriptorImageInfo imageInfo = {};
			imageInfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
			imageInfo.imageView = *vkTexture.GetImageView().Get();
			imageInfo.sampler = *m_sampler.Get();

			VkWriteDescriptorSet newWrite = {};
			newWrite.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
			newWrite.pNext = nullptr;
			newWrite.descriptorCount = 1;
			newWrite.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
			newWrite.pImageInfo = &imageInfo;
			newWrite.dstBinding = destinationBinding;
			newWrite.dstSet = *textureDescriptorSet->Get();

			m_device.UpdateDescriptorSetWrite(newWrite);
			textureDescriptorSet->WriteImageSamplerDescriptor(m_sampler, vkTexture.GetImageView());
		}
		return vkMaterialPtr;
	}

	void VkRHIMaterialBuilder::Update(const rhi::Buffer& buffer, UInt32 setIndex, UInt32 bindingIndex)
	{
		CCT_GFX_AUTO_PROFILER_SCOPE();

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
			//CCT_ASSERT("ConcertoGraphics: enum value not handled");
			return {};
		}();
		for (const auto& material : m_materialsCache)
		{
			const VkWriteDescriptorSet cameraWrite = VulkanInitializer::WriteDescriptorBuffer(
				descriptorType, *material->descriptorSets[setIndex]->Get(), &bufferInfo, bindingIndex);
			m_device.UpdateDescriptorSetWrite(cameraWrite);
		}
	}

	std::vector<VkPipelineShaderStageCreateInfo> VkRHIMaterialBuilder::GetShaderStages() const
	{
		std::vector<VkPipelineShaderStageCreateInfo> shaderStages;
		for (const auto& shaderModuleInfo : m_shaderModuleInfos | std::views::values)
			shaderStages.push_back(shaderModuleInfo.shaderModule->GetPipelineShaderStageCreateInfo());
		return shaderStages;
	}

	std::vector<std::shared_ptr<vk::DescriptorSetLayout>> VkRHIMaterialBuilder::GetDescriptorSetLayouts() const
	{
		std::vector<std::shared_ptr<vk::DescriptorSetLayout>> descriptorSetLayouts;

		for (const auto& layout : m_descriptorSetLayoutsCache | std::views::values)
			descriptorSetLayouts.push_back(layout);
		return descriptorSetLayouts;
	}

	ThreadSafeHashSet<vk::VkMaterialPtr> VkRHIMaterialBuilder::GetMaterials()
	{
		return m_materialsCache;
	}

	std::shared_ptr<vk::DescriptorSetLayout> VkRHIMaterialBuilder::GeDescriptorSetLayout(const std::vector<VkDescriptorSetLayoutBinding>& bindings)
	{
		CCT_GFX_AUTO_PROFILER_SCOPE();

		UInt64 hash = vk::DescriptorSetLayout::GetHash(bindings);

		const auto it = m_descriptorSetLayoutsCache.find(hash);
		if (it != m_descriptorSetLayoutsCache.end())
			return it->second;

		std::shared_ptr<vk::DescriptorSetLayout> descriptorSetLayout = MakeDescriptorSetLayout(m_device, bindings);
		m_descriptorSetLayoutsCache.emplace(hash, descriptorSetLayout);
		return descriptorSetLayout;
	}
}
