//
// Created by arthu on 14/09/2022.
//

#ifndef CONCERTOGRAPHICS_UPLOADCONTEXT_HPP
#define CONCERTOGRAPHICS_UPLOADCONTEXT_HPP

#include "Vulkan/Wrapper/CommandPool.hpp"
#include "Vulkan/Wrapper/CommandBuffer.hpp"
#include "Vulkan/Wrapper/Fence.hpp"
#include "Vulkan/Wrapper/Device.hpp"

namespace Concerto::Graphics
{
	struct UploadContext
	{
		UploadContext() = delete;

		UploadContext(Wrapper::Device& device, std::uint32_t queueFamily) : _uploadFence(device, false),
																			_commandPool(
																					device,
																					queueFamily),
																			_commandBuffer(
																					device,
																					*_commandPool.Get())
		{
		}

		~UploadContext() = default;

		UploadContext(UploadContext&&) = default;

		UploadContext(const UploadContext&) = delete;

		UploadContext& operator=(UploadContext&&) = default;

		UploadContext& operator=(const UploadContext&) = delete;

		Wrapper::Fence _uploadFence;
		Wrapper::CommandPool _commandPool;
		Wrapper::CommandBuffer _commandBuffer;
	};
}

#endif //CONCERTOGRAPHICS_UPLOADCONTEXT_HPP
