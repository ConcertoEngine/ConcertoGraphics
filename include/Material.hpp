//
// Created by arthur on 17/02/2023.
//

#ifndef CONCERTOGRAPHICS_INCLUDE_MATERIAL_HPP_
#define CONCERTOGRAPHICS_INCLUDE_MATERIAL_HPP_

#include <string>
#include <glm/vec3.hpp>

namespace Concerto::Graphics
{
	class Material
	{
	 public:
		std::string diffuseTexture;
		glm::vec3 diffuseColor;
		float metallic = 0.0f;
		glm::vec3 specular;
		float roughness = 0.0f;
		float anisotropy = 0.0f;
		glm::vec3 emissiveColor;
		std::string normalTexture;
	};
}
#endif //CONCERTOGRAPHICS_INCLUDE_MATERIAL_HPP_
