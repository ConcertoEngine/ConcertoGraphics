//
// Created by arthur on 13/09/2022.
//

#ifndef CONCERTO_GRAPHICS_MATERIAL_HPP
#define CONCERTO_GRAPHICS_MATERIAL_HPP

#include <memory>
#include <vector>

#include "Concerto/Graphics/Vulkan/Wrapper/DescriptorSet.hpp"
#include "Concerto/Graphics/Vulkan/Wrapper/Buffer.hpp"
#include "Concerto/Graphics/Vulkan/Wrapper/Pipeline.hpp"
#include "Concerto/Graphics/Vulkan/Texture.hpp"

namespace Concerto::Graphics
{
	struct CONCERTO_GRAPHICS_API VkMaterial
	{
		VkMaterial() = default;
		VkMaterial(VkMaterial&&other) = default;
		VkMaterial(VkMaterial&) = delete;
		VkMaterial& operator=(VkMaterial&& other) = default;
		VkMaterial& operator=(VkMaterial&) = delete;

		std::shared_ptr<Pipeline> pipeline;
		std::vector<DescriptorSetPtr> descriptorSets;
		TexturePtr diffuseTexture;
	};
	using VkMaterialPtr = std::shared_ptr<VkMaterial>;
}
#endif //CONCERTO_GRAPHICS_MATERIAL_HPP
