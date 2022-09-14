//
// Created by arthur on 13/09/2022.
//

#ifndef CONCERTOGRAPHICS_GPUDATA_HPP
#define CONCERTOGRAPHICS_GPUDATA_HPP

#include "glm/vec4.hpp"
#include "glm/mat4x4.hpp"
#include "wrapper/Fence.hpp"
#include "wrapper/CommandPool.hpp"
#include "wrapper/CommandBuffer.hpp"

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
		UploadContext(VkDevice device, std::uint32_t queueFamily, VkCommandPool commandPool) : _uploadFence(device),
																							   _commandPool(
																									   device,
																									   queueFamily),
																							   _commandBuffer(
																									   device,
																									   commandPool)
		{
		}

		Wrapper::Fence _uploadFence;
		Wrapper::CommandPool _commandPool;
		Wrapper::CommandBuffer _commandBuffer;
	};
}
#endif //CONCERTOGRAPHICS_GPUDATA_HPP
