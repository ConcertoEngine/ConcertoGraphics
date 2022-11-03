//
// Created by arthur on 13/09/2022.
//

#include <cassert>

#include "wrapper/Queue.hpp"
#include "wrapper/CommandBuffer.hpp"
#include "wrapper/Device.hpp"

namespace Concerto::Graphics::Wrapper
{
	Queue::Queue(Device& device, std::uint32_t queueFamilyIndex) : Object<VkQueue>(device), _queueFamilyIndex(queueFamilyIndex)
	{
		vkGetDeviceQueue(*_device->Get(), _queueFamilyIndex, 0, &_handle);
	}

	std::uint32_t Queue::GetFamilyIndex() const
	{
		return _queueFamilyIndex;
	}

	void Queue::Submit(const FrameData& frame)
	{
		VkCommandBuffer vkCommandBuffer = frame._mainCommandBuffer->Get();
		VkPipelineStageFlags waitStage = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
		VkSubmitInfo submit = {};
		submit.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
		submit.pNext = nullptr;
		submit.pWaitDstStageMask = &waitStage;
		submit.waitSemaphoreCount = 1;
		submit.pWaitSemaphores = frame._presentSemaphore.Get();
		submit.signalSemaphoreCount = 1;
		submit.pSignalSemaphores = frame._renderSemaphore.Get();
		submit.commandBufferCount = 1;
		submit.pCommandBuffers = &vkCommandBuffer;
		if (vkQueueSubmit(_handle, 1, &submit, *frame._renderFence.Get()) != VK_SUCCESS)
		{
			throw std::runtime_error("vkQueueSubmit fail");
		}
	}

	void Queue::Present(const FrameData& frame, Swapchain& swapchain, std::uint32_t swapchainImageIndex)
	{
		VkPresentInfoKHR present = {};
		present.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
		present.pNext = nullptr;
		present.pSwapchains = swapchain.Get();
		present.swapchainCount = 1;
		present.pWaitSemaphores = frame._renderSemaphore.Get();
		present.waitSemaphoreCount = 1;
		present.pImageIndices = &swapchainImageIndex;

		if (vkQueuePresentKHR(_handle, &present) != VK_SUCCESS)
		{
			throw std::runtime_error("vkQueuePresentKHR fail");
		}
	}
}

