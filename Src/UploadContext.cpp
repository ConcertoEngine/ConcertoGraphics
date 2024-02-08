//
// Created by arthu on 22/08/2022.
//

#include "Concerto/Graphics/UploadContext.hpp"
#include "Concerto/Graphics/Vulkan/Wrapper/Device.hpp"

namespace Concerto::Graphics
{
	UploadContext::UploadContext(Device& device, UInt32 queueFamily) :
		_uploadFence(device, false),
		_commandPool(device, queueFamily),
		_commandBuffer(device, *_commandPool.Get())
	{
	}
}