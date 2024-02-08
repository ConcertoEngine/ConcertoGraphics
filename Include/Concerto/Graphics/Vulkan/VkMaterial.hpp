//
// Created by arthur on 13/09/2022.
//

#ifndef CONCERTO_GRAPHICS_MATERIAL_HPP
#define CONCERTO_GRAPHICS_MATERIAL_HPP

#include <memory>
#include <vector>

namespace Concerto::Graphics
{
	class DescriptorSet;
	class Pipeline;
	class Texture;

	struct CONCERTO_GRAPHICS_API VkMaterial
	{
		VkMaterial() = default;
		VkMaterial(VkMaterial&&other) = default;
		VkMaterial(VkMaterial&) = delete;
		VkMaterial& operator=(VkMaterial&& other) = default;
		VkMaterial& operator=(VkMaterial&) = delete;

		std::shared_ptr<Pipeline> pipeline;
		std::vector<std::shared_ptr<DescriptorSet>> descriptorSets;
		std::shared_ptr<Texture> diffuseTexture;
	};
	using VkMaterialPtr = std::shared_ptr<VkMaterial>;
}
#endif //CONCERTO_GRAPHICS_MATERIAL_HPP
