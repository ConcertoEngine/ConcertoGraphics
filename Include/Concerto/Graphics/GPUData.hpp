//
// Created by arthur on 13/09/2022.
//

#ifndef CONCERTO_GRAPHICS_GPUDATA_HPP
#define CONCERTO_GRAPHICS_GPUDATA_HPP

#include <glm/vec4.hpp>
#include <glm/mat4x4.hpp>

namespace Concerto::Graphics
{
	struct GPUCamera
	{
		glm::mat4 viewMatrix;
		glm::mat4 projectionMatrix;
		glm::mat4 viewProjectionMatrix;
	};

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
#endif //CONCERTO_GRAPHICS_GPUDATA_HPP
