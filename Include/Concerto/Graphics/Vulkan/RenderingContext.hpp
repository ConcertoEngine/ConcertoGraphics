//
// Created by arthur on 21/12/2022.
//

#ifndef CONCERTOGRAPHICS_RENDERINGCONTEXT_HPP
#define CONCERTOGRAPHICS_RENDERINGCONTEXT_HPP

#include <cstdint>
#include <vulkan/vulkan.h>

namespace Concerto::Graphics
{
	class Instance;
	class PhysicalDevice;
	class Device;
	class Queue;
	class Pipeline;
	class DescriptorPool;
	class RenderPass;
	class Allocator;
	class CommandBuffer;
	class Fence;
	class CommandPool;

	struct RenderingContext
	{
		Instance* instance;
		PhysicalDevice* physicalDevice;
		Device* device;
		std::uint32_t queueFamilyIndex;
		Queue* queue;
		Pipeline* pipeline;
		RenderPass* renderPass;
		std::uint32_t subPass;
		std::uint32_t minImageCount;
		std::uint32_t imageCount;
		VkSampleCountFlagBits MSAASamples;
		CommandBuffer* commandBuffer;
		Fence* fence;
		CommandPool* commandPool;
	};
} // Concerto::Graphics
#endif //CONCERTOGRAPHICS_RENDERINGCONTEXT_HPP
