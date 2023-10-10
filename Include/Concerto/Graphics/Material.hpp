//
// Created by arthur on 17/02/2023.
//

#ifndef CONCERTOGRAPHICS_INCLUDE_MATERIAL_HPP_
#define CONCERTOGRAPHICS_INCLUDE_MATERIAL_HPP_

#include <string>
#include <memory>

#include <glm/vec3.hpp>
#include <glm/gtx/hash.hpp>

#include "Concerto/Graphics/Vulkan/Texture.hpp"

namespace Concerto::Graphics
{
	class MaterialInfo
	{
	 public:
		struct Hash
		{
			std::size_t operator()(const MaterialInfo& material) const
			{
				std::size_t h1 = std::hash<std::string>{}(material.diffuseTexturePath);
				std::size_t h2 = std::hash<glm::vec3>{}(material.diffuseColor);
				std::size_t h3 = std::hash<float>{}(material.metallic);
				std::size_t h4 = std::hash<glm::vec3>{}(material.specular);
				std::size_t h5 = std::hash<float>{}(material.roughness);
				std::size_t h6 = std::hash<float>{}(material.anisotropy);
				std::size_t h7 = std::hash<glm::vec3>{}(material.emissiveColor);
				std::size_t h8 = std::hash<std::string>{}(material.normalTexturePath);
				return h1 ^ (h2 << 1) ^ (h3 << 2) ^ (h4 << 3) ^ (h5 << 4) ^ (h6 << 5) ^ (h7 << 6) ^ (h8 << 7);
			}
		};
		MaterialInfo() = default;
		bool operator==(const MaterialInfo& other) const
		{
			return diffuseTexture == other.diffuseTexture && diffuseColor == other.diffuseColor
				&& metallic == other.metallic && specular == other.specular && roughness == other.roughness
				&& anisotropy == other.anisotropy && emissiveColor == other.emissiveColor
				&& normalTexture == other.normalTexture;
		}

		TexturePtr diffuseTexture;
		std::string diffuseTexturePath;
		glm::vec3 diffuseColor;
		float metallic = 0.0f;
		glm::vec3 specular;
		float roughness = 0.0f;
		float anisotropy = 0.0f;
		glm::vec3 emissiveColor;
		TexturePtr normalTexture;
		std::string normalTexturePath;
		std::string name;
		std::string vertexShaderPath;
		std::string fragmentShaderPath;
	};
	using MaterialPtr = std::shared_ptr<MaterialInfo>;
}
#endif //CONCERTOGRAPHICS_INCLUDE_MATERIAL_HPP_
