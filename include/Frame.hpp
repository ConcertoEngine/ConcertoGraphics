//
// Created by arthur on 13/09/2022.
//

#ifndef CONCERTOGRAPHICS_FRAME_HPP
#define CONCERTOGRAPHICS_FRAME_HPP

#include <memory>
#include "wrapper/AllocatedBuffer.hpp"
#include "wrapper/Allocator.hpp"
#include "wrapper/DescriptorSet.hpp"
#include "wrapper/DescriptorSetLayout.hpp"
#include "wrapper/DescriptorPool.hpp"
#include "wrapper/Fence.hpp"
#include "wrapper/Semaphore.hpp"
#include "GPUData.hpp"

#define MAX_OBJECTS 1000

namespace Concerto::Graphics
{
	namespace Wrapper
	{
		class CommandBuffer;
		class CommandPool;
		class Device;
	}
	struct FrameData
	{
		FrameData(Wrapper::Device& device,Wrapper::Allocator& allocator, std::uint32_t queueFamily, Wrapper::DescriptorPool& pool,
				Wrapper::DescriptorSetLayout& globalDescriptorSetLayout, Wrapper::DescriptorSetLayout& objectDescriptorSetLayout,
				Wrapper::AllocatedBuffer& sceneParameterBuffer,
				bool signaled = true);

		FrameData(FrameData&&) = default;

		FrameData() = delete;

		~FrameData() = default;

		Wrapper::Semaphore _presentSemaphore, _renderSemaphore;
		Wrapper::Fence _renderFence;

		std::unique_ptr<Wrapper::CommandPool> _commandPool;
		std::unique_ptr<Wrapper::CommandBuffer> _mainCommandBuffer;

		Wrapper::AllocatedBuffer _cameraBuffer;
		Wrapper::DescriptorSet globalDescriptor;

		Wrapper::AllocatedBuffer _objectBuffer;
		Wrapper::DescriptorSet objectDescriptor;
	};
}
#endif //CONCERTOGRAPHICS_FRAME_HPP
