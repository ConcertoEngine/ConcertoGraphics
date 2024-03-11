//
// Created by arthur on 21/12/2022.
//

#ifndef CONCERTO_GRAPHICS_RENDERINGCONTEXT_HPP
#define CONCERTO_GRAPHICS_RENDERINGCONTEXT_HPP

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
	class RenderPass;

	struct RenderingContext
	{
		Instance* instance;
		PhysicalDevice* physicalDevice;
		Device* device;
		UInt32 queueFamilyIndex;
		Queue* queue;
		UInt32 minImageCount;
		UInt32 imageCount;
		VkSampleCountFlagBits MSAASamples;
		CommandBuffer* commandBuffer;
		Fence* fence;
		CommandPool* commandPool;
		RenderPass* renderPass;
	};
} // Concerto::Graphics
#endif //CONCERTO_GRAPHICS_RENDERINGCONTEXT_HPP
