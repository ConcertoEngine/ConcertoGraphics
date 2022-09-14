//
// Created by arthur on 13/09/2022.
//

#ifndef CONCERTOGRAPHICS_GPUDATA_HPP
#define CONCERTOGRAPHICS_GPUDATA_HPP

#include "glm/vec4.hpp"
#include "glm/mat4x4.hpp"
#include "vulkan/vulkan.h"

namespace Concerto::Graphics
{
	struct GPUCameraData
	{
		glm::mat4 view;
		glm::mat4 proj;
		glm::mat4 viewproj;
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

	struct UploadContext
	{
		VkFence _uploadFence = VK_NULL_HANDLE;
		VkCommandPool _commandPool = VK_NULL_HANDLE;
		VkCommandBuffer _commandBuffer = VK_NULL_HANDLE;
	};
}
#endif //CONCERTOGRAPHICS_GPUDATA_HPP
