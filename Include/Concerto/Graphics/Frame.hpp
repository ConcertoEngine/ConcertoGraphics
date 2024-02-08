//
// Created by arthur on 13/09/2022.
//

#ifndef CONCERTO_GRAPHICS_FRAME_HPP
#define CONCERTO_GRAPHICS_FRAME_HPP

#include <memory>

#include "Concerto/Graphics/Defines.hpp"

#include "Concerto/Graphics/Vulkan/Wrapper/Buffer.hpp"
#include "Concerto/Graphics/Vulkan/Wrapper/Fence.hpp"
#include "Concerto/Graphics/Vulkan/Wrapper/Semaphore.hpp"
#include "Concerto/Graphics/Vulkan/Wrapper/CommandBuffer.hpp"
#include "Concerto/Graphics/Vulkan/Wrapper/CommandPool.hpp"

#define MAX_OBJECTS 1000

namespace Concerto::Graphics
{
	class Device;

	class CONCERTO_GRAPHICS_API FrameData
	{
	public:
		FrameData() = delete;

		explicit FrameData(Device& device, bool signaled = true);

		~FrameData() = default;

		FrameData(FrameData&&) = default;

		FrameData(FrameData&) = delete;

		FrameData& operator=(FrameData&&) = default;
		FrameData& operator=(FrameData&) = delete;
		
		Semaphore presentSemaphore, renderSemaphore;
		Fence renderFence;

		CommandPool commandPool;
		CommandBuffer commandBuffer;

		Buffer indirectBuffer;
		bool isResized;
	};
}
#endif //CONCERTO_GRAPHICS_FRAME_HPP
