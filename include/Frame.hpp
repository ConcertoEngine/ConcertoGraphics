//
// Created by arthur on 13/09/2022.
//

#ifndef CONCERTOGRAPHICS_FRAME_HPP
#define CONCERTOGRAPHICS_FRAME_HPP

#include "wrapper/Allocator.hpp"
#include "wrapper/DescriptorSetLayout.hpp"
#include "wrapper/Semaphore.hpp"
#include "wrapper/Fence.hpp"
#include "wrapper/CommandBuffer.hpp"
#include "wrapper/CommandPool.hpp"

#include "GPUData.hpp"

#define MAX_OBJECTS 1000

namespace Concerto::Graphics
{
	struct FrameData
	{
		FrameData(Wrapper::Allocator& allocator, VkDevice device, std::uint32_t queueFamily, Wrapper::DescriptorPool& pool,
				Wrapper::DescriptorSetLayout& globalDescriptorSetLayout, Wrapper::DescriptorSetLayout& objectDescriptorSetLayout,
				Wrapper::AllocatedBuffer& sceneParameterBuffer,
				bool signaled = true);

		FrameData(FrameData&&) = default;

		FrameData() = delete;

		~FrameData() = default;

		Wrapper::Semaphore _presentSemaphore, _renderSemaphore;
		Wrapper::Fence _renderFence;

		Wrapper::CommandPool _commandPool;
		Wrapper::CommandBuffer _mainCommandBuffer;

		Wrapper::AllocatedBuffer _cameraBuffer;
		Wrapper::DescriptorSet globalDescriptor;

		Wrapper::AllocatedBuffer _objectBuffer;
		Wrapper::DescriptorSet objectDescriptor;
	};
}
#endif //CONCERTOGRAPHICS_FRAME_HPP
