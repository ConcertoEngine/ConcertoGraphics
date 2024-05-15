//
// Created by arthu on 22/08/2022.
//

#include "Concerto/Graphics/UploadContext.hpp"
#include "Concerto/Graphics/Backend/Vulkan/Wrapper/Device.hpp"

namespace Concerto::Graphics::Vk
{
	UploadContext::UploadContext(Device& device, UInt32 queueFamily) :
		_uploadFence(device, false),
		_commandPool(device, queueFamily),
		_commandBuffer(device, *_commandPool.Get())
	{
	}
}