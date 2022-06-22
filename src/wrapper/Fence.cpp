//
// Created by arthur on 15/06/22.
//


#include "wrapper/Fence.hpp"
#include <stdexcept>
namespace Concerto::Graphics::Wrapper
{

	Fence::Fence(VkDevice device, bool signaled) : _device(device)
	{
		VkFenceCreateInfo info = {};
		info.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
		info.pNext = nullptr;
		info.flags = signaled ? VK_FENCE_CREATE_SIGNALED_BIT : 0;
		if (vkCreateFence(_device, &info, nullptr, &_fence) != VK_SUCCESS)
		{
			throw std::runtime_error("Failed to create fence");
		}
	}

	Fence::~Fence()
	{
		vkDestroyFence(_device, _fence, nullptr);
	}

	VkFence Fence::get() const
	{
		return _fence;
	}
} // namespace Concerto::Graphics::Wrapper