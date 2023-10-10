//
// Created by arthu on 14/09/2022.
//

#ifndef CONCERTOGRAPHICS_UPLOADCONTEXT_HPP
#define CONCERTOGRAPHICS_UPLOADCONTEXT_HPP

#include "Concerto/Graphics/Vulkan/Wrapper/CommandPool.hpp"
#include "Concerto/Graphics/Vulkan/Wrapper/CommandBuffer.hpp"
#include "Concerto/Graphics/Vulkan/Wrapper/Fence.hpp"

namespace Concerto::Graphics
{
	struct UploadContext
	{
		UploadContext() = delete;

		UploadContext(Device& device, UInt32 queueFamily);

		~UploadContext() = default;

		UploadContext(UploadContext&&) = default;

		UploadContext(const UploadContext&) = delete;

		UploadContext& operator=(UploadContext&&) = default;

		UploadContext& operator=(const UploadContext&) = delete;

		Fence _uploadFence;
		CommandPool _commandPool;
		CommandBuffer _commandBuffer;
	};
}

#endif //CONCERTOGRAPHICS_UPLOADCONTEXT_HPP
