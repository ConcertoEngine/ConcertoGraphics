//
// Created by arthu on 22/08/2022.
//

#include "Concerto/Graphics/Backend/Vulkan/UploadContext.hpp"
#include "Concerto/Graphics/Backend/Vulkan/Wrapper/Device.hpp"

namespace cct::gfx::vk
{
	UploadContext::UploadContext(Device& device, UInt32 queueFamily) :
		_uploadFence(device, false),
		_commandPool(device, queueFamily),
		_commandBuffer(_commandPool.AllocateCommandBuffer(VK_COMMAND_BUFFER_LEVEL_PRIMARY)),
		_nextSecondaryCommandBuffer(0)
	{
#ifdef CCT_ENABLE_OBJECT_DEBUG
		_commandBuffer.SetDebugName("UploadContextCommandBuffer");
#endif
	}

	void UploadContext::ReserveSecondaryCommandBuffers(UInt32 count)
	{
		CCT_GFX_AUTO_PROFILER_SCOPE();

		_secondCommandBuffers.reserve(count);
		// count - 1 because we start at 0
		for (UInt32 i = 0; i < count - 1; ++i)
		{
			auto cb = _commandPool.AllocateCommandBuffer(VK_COMMAND_BUFFER_LEVEL_SECONDARY);
#ifdef CCT_ENABLE_OBJECT_DEBUG
			cb.SetDebugName(std::format("UploadContext::_secondCommandBuffers[{}]", i));
#endif
			_secondCommandBuffers.emplace_back(std::move(cb));
		}
	}

	CommandBuffer* UploadContext::AcquireSecondaryCommandBuffer()
	{
		if (_nextSecondaryCommandBuffer >= _secondCommandBuffers.size())
			return nullptr; // Exhausted
		return &_secondCommandBuffers[_nextSecondaryCommandBuffer++];
	}

	void UploadContext::ExecuteSecondaryCommandBuffers()
	{
		_commandBuffer.ExecuteCommands(_secondCommandBuffers);
	}
}
