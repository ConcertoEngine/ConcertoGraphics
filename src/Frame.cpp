//
// Created by arthur on 13/09/2022.
//

#include "Frame.hpp"
#include "wrapper/VulkanInitializer.hpp"
#include "wrapper/CommandBuffer.hpp"
#include "wrapper/CommandPool.hpp"

namespace Concerto::Graphics
{

	Graphics::FrameData::FrameData(Wrapper::Allocator& allocator, VkDevice device, std::uint32_t queueFamily,
			Wrapper::DescriptorPool& pool, Wrapper::DescriptorSetLayout& globalDescriptorSetLayout,
			Wrapper::DescriptorSetLayout& objectDescriptorSetLayout, Wrapper::AllocatedBuffer& sceneParameterBuffer,
			bool signaled) : _presentSemaphore(device),
							 _commandPool(std::make_unique<Wrapper::CommandPool>(device, queueFamily)),
							 _renderSemaphore(device),
							 _renderFence(device, signaled),
							 _mainCommandBuffer(std::make_unique<Wrapper::CommandBuffer>(device, _commandPool->Get())),
							 _cameraBuffer(MakeAllocatedBuffer<GPUCameraData>(allocator,
									 VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT,
									 VMA_MEMORY_USAGE_CPU_TO_GPU)),
							 globalDescriptor(device, pool, globalDescriptorSetLayout),
							 _objectBuffer(MakeAllocatedBuffer<GPUObjectData>(allocator, MAX_OBJECTS,
									 VK_BUFFER_USAGE_STORAGE_BUFFER_BIT,
									 VMA_MEMORY_USAGE_CPU_TO_GPU)),
							 objectDescriptor(device, pool, objectDescriptorSetLayout)
	{

		VkDescriptorBufferInfo cameraInfo;
		cameraInfo.buffer = _cameraBuffer._buffer;
		cameraInfo.offset = 0;
		cameraInfo.range = sizeof(GPUCameraData);

		VkDescriptorBufferInfo sceneInfo;
		sceneInfo.buffer = sceneParameterBuffer._buffer;
		sceneInfo.offset = 0;
		sceneInfo.range = sizeof(GPUSceneData);

		VkDescriptorBufferInfo objectBufferInfo;
		objectBufferInfo.buffer = _objectBuffer._buffer;
		objectBufferInfo.offset = 0;
		objectBufferInfo.range = sizeof(GPUObjectData) * MAX_OBJECTS;

		VkWriteDescriptorSet cameraWrite = VulkanInitializer::WriteDescriptorBuffer(
				VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,
				*globalDescriptor.Get(), &cameraInfo, 0);

		VkWriteDescriptorSet sceneWrite = VulkanInitializer::WriteDescriptorBuffer(
				VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC, *globalDescriptor.Get(), &sceneInfo, 1);

		VkWriteDescriptorSet objectWrite = VulkanInitializer::WriteDescriptorBuffer(
				VK_DESCRIPTOR_TYPE_STORAGE_BUFFER,
				*objectDescriptor.Get(), &objectBufferInfo, 0);

		VkWriteDescriptorSet setWrites[] = { cameraWrite, sceneWrite, objectWrite };

		vkUpdateDescriptorSets(device, 3, setWrites, 0, nullptr);
	}
}