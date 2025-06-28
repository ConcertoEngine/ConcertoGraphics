//
// Created by arthur on 17/02/2023.
//

#ifndef CONCERTO_GRAPHICS_RHI_VULKAN_VKRHIMATERIALBUILDER_HPP
#define CONCERTO_GRAPHICS_RHI_VULKAN_VKRHIMATERIALBUILDER_HPP

#include <set>
#include <string>

#include "Concerto/Graphics/RHI/Defines.hpp"
#include "Concerto/Graphics/RHI/MaterialBuilder.hpp"
#include "Concerto/Graphics/RHI/Material.hpp"
#include "Concerto/Graphics/RHI/Buffer.hpp"
#include "Concerto/Graphics/RHI/Vulkan/VkMaterial.hpp"

#include "Concerto/Graphics/Backend/Vulkan/DescriptorAllocator.hpp"
#include "Concerto/Graphics/Backend/Vulkan/ShaderModuleInfo.hpp"
#include "Concerto/Graphics/Backend/Vulkan/Wrapper/Sampler.hpp"
#include "Concerto/Graphics/Backend/Vulkan/Wrapper/Pipeline.hpp"

namespace cct::gfx::vk
{
	class RenderPass;
	class DescriptorLayoutCache;
}

namespace cct::gfx::rhi
{
	class CONCERTO_GRAPHICS_RHI_BASE_API VkRHIMaterialBuilder : public rhi::MaterialBuilder
	{
	 public:
		explicit VkRHIMaterialBuilder(vk::Device& device, VkExtent2D windowExtent);
		~VkRHIMaterialBuilder() override = default;
		MaterialPtr BuildMaterial(rhi::MaterialInfo& material, const rhi::RenderPass& renderPass) override;
		void Update(const rhi::Buffer& buffer, UInt32 setIndex, UInt32 bindingIndex) override;

		[[nodiscard]] std::vector<VkPipelineShaderStageCreateInfo> GetShaderStages() const;
		[[nodiscard]] std::vector<std::shared_ptr<vk::DescriptorSetLayout>> GetDescriptorSetLayouts() const;
		ThreadSafeHashSet<vk::VkMaterialPtr> GetMaterials();
	private:
		std::shared_ptr<vk::DescriptorSetLayout> GeDescriptorSetLayout(const std::vector<VkDescriptorSetLayoutBinding>& bindings);

		vk::DescriptorAllocator _allocator;
		vk::Device& _device;
		ThreadSafeHashSet<vk::VkMaterialPtr> _materialsCache;
		ThreadSafeHashMap<UInt64 /*hash*/, std::shared_ptr<vk::Pipeline>> _pipelinesCache;
		ThreadSafeHashMap<std::string, vk::ShaderModuleInfo> _shaderModuleInfos;
		vk::DescriptorPool _descriptorPool;
		ThreadSafeHashMap<UInt64 /*hash*/, std::shared_ptr<vk::DescriptorSetLayout>> _descriptorSetLayoutsCache;
		std::vector<vk::Pipeline> _pipelines;
		vk::Sampler _sampler;
		VkExtent2D _windowExtent;
	};
}
#endif //CONCERTO_GRAPHICS_RHI_VULKAN_VKRHIMATERIALBUILDER_HPP
