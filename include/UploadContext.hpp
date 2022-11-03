//
// Created by arthu on 14/09/2022.
//

#ifndef CONCERTOGRAPHICS_UPLOADCONTEXT_HPP
#define CONCERTOGRAPHICS_UPLOADCONTEXT_HPP

#include "wrapper/CommandPool.hpp"
#include "wrapper/CommandBuffer.hpp"
#include "wrapper/Fence.hpp"
#include "wrapper/Device.hpp"

namespace Concerto::Graphics
{
	struct UploadContext
	{
		UploadContext(Wrapper::Device& device, std::uint32_t queueFamily) : _uploadFence(device, false),
																	_commandPool(
																			device,
																			queueFamily),
																	_commandBuffer(
																			device,
																			*_commandPool.Get())
		{
		}

		Wrapper::Fence _uploadFence;
		Wrapper::CommandPool _commandPool;
		Wrapper::CommandBuffer _commandBuffer;
	};
}

#endif //CONCERTOGRAPHICS_UPLOADCONTEXT_HPP
