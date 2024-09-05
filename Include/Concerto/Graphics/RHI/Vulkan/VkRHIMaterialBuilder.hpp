//
// Created by arthur on 17/02/2023.
//

#ifndef CONCERTO_GRAPHICS_RHI_VULKAN_VKRHIMATERIALBUILDER_HPP
#define CONCERTO_GRAPHICS_RHI_VULKAN_VKRHIMATERIALBUILDER_HPP

#include <set>
#include <string>
#include <unordered_map>

#include "Concerto/Graphics/RHI/MaterialBuilder.hpp"
#include "Concerto/Graphics/RHI/Material.hpp"
#include "Concerto/Graphics/Backend/Vulkan/VkMaterial.hpp"
#include "Concerto/Graphics/Backend/Vulkan/Wrapper/Sampler.hpp"
#include "Concerto/Graphics/Backend/Vulkan/DescriptorAllocator.hpp"
#include "Concerto/Graphics/ShaderModuleInfo.hpp"
#include "Concerto/Graphics/Backend/Vulkan/Wrapper/Pipeline.hpp"

namespace Concerto::Graphics::Vk
{
	class RenderPass;
	class DescriptorLayoutCache;
}

namespace Concerto::Graphics::RHI
{
	class CONCERTO_GRAPHICS_VKRHI_API VkRHIMaterialBuilder : public RHI::MaterialBuilder
	{
	 public:
		explicit VkRHIMaterialBuilder(Vk::Device& device, VkExtent2D windowExtent);
		MaterialPtr BuildMaterial(RHI::MaterialInfo& material, const RHI::RenderPass& renderPass) override;

		[[nodiscard]] std::vector<VkPipelineShaderStageCreateInfo> GetShaderStages() const;
		[[nodiscard]] std::vector<std::shared_ptr<Vk::DescriptorSetLayout>> GetDescriptorSetLayouts() const;
		std::set<Vk::VkMaterialPtr> GetMaterials();
	 private:
		std::shared_ptr<Vk::DescriptorSetLayout> GeDescriptorSetLayout(const std::vector<VkDescriptorSetLayoutBinding>& bindings);

		Vk::DescriptorAllocator _allocator;
		Vk::Device& _device;
		std::set<Vk::VkMaterialPtr> _materialsCache;
		std::unordered_map<UInt64 /*hash*/, std::shared_ptr<Vk::Pipeline>> _pipelinesCache;
		std::unordered_map<std::string, Vk::ShaderModuleInfo> _shaderModuleInfos;
		Vk::DescriptorPool _descriptorPool;
		std::unordered_map<UInt64 /*hash*/, std::shared_ptr<Vk::DescriptorSetLayout>> _descriptorSetLayoutsCache;
		std::vector<Vk::Pipeline> _pipelines;
		Vk::Sampler _sampler;
		VkExtent2D _windowExtent;
	};
}
#endif //CONCERTO_GRAPHICS_RHI_VULKAN_VKRHIMATERIALBUILDER_HPP
