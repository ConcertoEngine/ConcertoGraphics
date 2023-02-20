//
// Created by arthur on 13/09/2022.
//

#ifndef CONCERTOGRAPHICS_FRAME_HPP
#define CONCERTOGRAPHICS_FRAME_HPP

#include <memory>
#include "Vulkan/Wrapper/Buffer.hpp"
#include "Vulkan/Wrapper/Allocator.hpp"
#include "Vulkan/Wrapper/DescriptorSet.hpp"
#include "Vulkan/Wrapper/DescriptorSetLayout.hpp"
#include "Vulkan/Wrapper/DescriptorPool.hpp"
#include "Vulkan/Wrapper/Fence.hpp"
#include "Vulkan/Wrapper/Semaphore.hpp"
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
				Wrapper::Buffer& sceneParameterBuffer,
				bool signaled = true);

		FrameData(FrameData&&) = default;

		FrameData() = delete;

		~FrameData() = default;

		Wrapper::Semaphore _presentSemaphore, _renderSemaphore;
		Wrapper::Fence _renderFence;

		std::unique_ptr<Wrapper::CommandPool> _commandPool;
		std::unique_ptr<Wrapper::CommandBuffer> _mainCommandBuffer;

		Wrapper::Buffer _cameraBuffer;
		Wrapper::DescriptorSet globalDescriptor;

		Wrapper::Buffer _objectBuffer;
		Wrapper::DescriptorSet objectDescriptor;

		Wrapper::Buffer _indirectBuffer;
		bool _isResized = true;
	};
}
#endif //CONCERTOGRAPHICS_FRAME_HPP
