//
// Created by arthur on 13/09/2022.
//

#include "Frame.hpp"
#include "Vulkan/Wrapper/VulkanInitializer.hpp"
#include "Vulkan/Wrapper/CommandBuffer.hpp"
#include "Vulkan/Wrapper/CommandPool.hpp"
#include "Vulkan/Wrapper/Device.hpp"

namespace Concerto::Graphics
{
	using namespace Concerto::Graphics;
	Graphics::FrameData::FrameData(Device& device, Allocator& allocator, std::uint32_t queueFamily,
			DescriptorPool& pool, DescriptorSetLayout& globalDescriptorSetLayout,
			DescriptorSetLayout& objectDescriptorSetLayout, Buffer& sceneParameterBuffer,
			bool signaled) : _presentSemaphore(device),
							 _commandPool(std::make_unique<CommandPool>(device, queueFamily)),
							 _renderSemaphore(device),
							 _renderFence(device, signaled),
							 _mainCommandBuffer(std::make_unique<CommandBuffer>(device, *_commandPool->Get())),
							 _cameraBuffer(MakeBuffer<GPUCamera>(allocator,
								 VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT,
								 VMA_MEMORY_USAGE_CPU_TO_GPU)),
							 globalDescriptor(device, pool, globalDescriptorSetLayout),
							 _objectBuffer(MakeBuffer<GPUObjectData>(allocator, MAX_OBJECTS,
								 VK_BUFFER_USAGE_STORAGE_BUFFER_BIT,
								 VMA_MEMORY_USAGE_CPU_TO_GPU)),
							 objectDescriptor(device, pool, objectDescriptorSetLayout),
							 _indirectBuffer(MakeBuffer<VkDrawIndirectCommand>(allocator,
								 MAX_OBJECTS,
								 VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_STORAGE_BUFFER_BIT |  VK_BUFFER_USAGE_INDIRECT_BUFFER_BIT,
								 VMA_MEMORY_USAGE_CPU_TO_GPU))
	{

		VkDescriptorBufferInfo cameraInfo;
		cameraInfo.buffer = _cameraBuffer._buffer;
		cameraInfo.offset = 0;
		cameraInfo.range = sizeof(GPUCamera);

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

		vkUpdateDescriptorSets(*device.Get(), 3, setWrites, 0, nullptr);
	}
}