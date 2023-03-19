//
// Created by arthur on 13/09/2022.
//

#ifndef CONCERTOGRAPHICS_GPUDATA_HPP
#define CONCERTOGRAPHICS_GPUDATA_HPP

#include <glm/vec4.hpp>
#include <glm/mat4x4.hpp>

namespace Concerto::Graphics
{
	struct Camera
	{
		glm::mat4 view;
		glm::mat4 proj;
		glm::mat4 viewproj;
	};
	using GPUCameraData = Camera;

	struct GPUObjectData
	{
		glm::mat4 modelMatrix;
	};

	struct GPUSceneData
	{
		glm::vec4 fogColor; // w is for exponent
		glm::vec4 fogDistances; //x for min, y for max, zw unused.
		glm::vec4 ambientColor;
		glm::vec4 sunlightDirection; //w for sun power
		glm::vec4 sunlightColor;
	};

	struct Scene
	{
		GPUSceneData gpuSceneData;
		glm::vec4 clearColor;
	};
}
#endif //CONCERTOGRAPHICS_GPUDATA_HPP
