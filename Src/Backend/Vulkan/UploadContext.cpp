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
		_commandBuffer(_commandPool.AllocateCommandBuffer())
	{
#ifdef CCT_DEBUG
		_commandBuffer.SetDebugName("UploadContextCommandBuffer");
#endif
	}
}