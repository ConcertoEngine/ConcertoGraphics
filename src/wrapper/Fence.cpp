//
// Created by arthur on 15/06/22.
//


#include <stdexcept>
#include "wrapper/Fence.hpp"

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
		_fence = VK_NULL_HANDLE;
	}

	VkFence Fence::Get() const
	{
		return _fence;
	}

	void Fence::wait(std::uint64_t timeout)
	{
		if (vkWaitForFences(_device, 1, &_fence, true, timeout) != VK_SUCCESS)
		{
			throw std::runtime_error("vkWaitForFences fail");
		}
	}

	void Fence::reset()
	{
		if (vkResetFences(_device, 1, &_fence) != VK_SUCCESS)
		{
			throw std::runtime_error("vkResetFences fail");
		}
	}
} // namespace Concerto::Graphics::Wrapper