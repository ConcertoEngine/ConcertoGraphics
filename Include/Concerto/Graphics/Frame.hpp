//
// Created by arthur on 13/09/2022.
//

#ifndef CONCERTOGRAPHICS_FRAME_HPP
#define CONCERTOGRAPHICS_FRAME_HPP

#include <memory>

#include <Concerto/Core/Types.hpp>

#include "Concerto/Graphics/Vulkan/Wrapper/Buffer.hpp"
#include "Concerto/Graphics/Vulkan/Wrapper/Allocator.hpp"
#include "Concerto/Graphics/Vulkan/Wrapper/DescriptorSet.hpp"
#include "Concerto/Graphics/Vulkan/Wrapper/DescriptorSetLayout.hpp"
#include "Concerto/Graphics/Vulkan/Wrapper/DescriptorPool.hpp"
#include "Concerto/Graphics/Vulkan/Wrapper/Fence.hpp"
#include "Concerto/Graphics/Vulkan/Wrapper/Semaphore.hpp"
#include "Concerto/Graphics/GPUData.hpp"

#define MAX_OBJECTS 1000

namespace Concerto::Graphics
{
	class CommandBuffer;
	class CommandPool;
	class Device;

	struct FrameData
	{
		FrameData(Device& device,Allocator& allocator, std::uint32_t queueFamily, DescriptorPool& pool,
				DescriptorSetLayout& globalDescriptorSetLayout, DescriptorSetLayout& objectDescriptorSetLayout,
				Buffer& sceneParameterBuffer,
				bool signaled = true);

		FrameData(FrameData&&) = default;

		FrameData() = delete;

		~FrameData() = default;

		Semaphore _presentSemaphore, _renderSemaphore;
		Fence _renderFence;

		std::unique_ptr<CommandPool> _commandPool;
		std::unique_ptr<CommandBuffer> _mainCommandBuffer;

		Buffer _cameraBuffer;
		DescriptorSet globalDescriptor;

		Buffer _objectBuffer;
		DescriptorSet objectDescriptor;

		Buffer _indirectBuffer;
		bool _isResized = true;
	};
}
#endif //CONCERTOGRAPHICS_FRAME_HPP
