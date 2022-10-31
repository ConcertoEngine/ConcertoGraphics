//
// Created by arthur on 13/09/2022.
//

#include <cassert>

#include "wrapper/Queue.hpp"
#include "wrapper/CommandBuffer.hpp"
#include "wrapper/Device.hpp"

namespace Concerto::Graphics::Wrapper
{
	Queue::Queue(Device& device, std::uint32_t queueFamilyIndex) : _device(device), _queueFamilyIndex(queueFamilyIndex)
	{
		vkGetDeviceQueue(*_device.Get(), _queueFamilyIndex, 0, &_queue);
	}

	std::uint32_t Queue::GetFamilyIndex() const
	{
		return _queueFamilyIndex;
	}

	void Queue::Submit(const FrameData& frame)
	{
		VkPipelineStageFlags waitStage = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
		auto vkPresentSemaphore = frame._presentSemaphore.Get();
		auto vkRenderSemaphore = frame._renderSemaphore.Get();
		auto vkCommandBuffer = frame._mainCommandBuffer->Get();
		VkSubmitInfo submit = {};
		submit.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
		submit.pNext = nullptr;
		submit.pWaitDstStageMask = &waitStage;
		submit.waitSemaphoreCount = 1;
		submit.pWaitSemaphores = &vkPresentSemaphore;
		submit.signalSemaphoreCount = 1;
		submit.pSignalSemaphores = &vkRenderSemaphore;
		submit.commandBufferCount = 1;
		submit.pCommandBuffers = &vkCommandBuffer;
		if (vkQueueSubmit(_queue, 1, &submit, frame._renderFence.Get()) != VK_SUCCESS)
		{
			throw std::runtime_error("vkQueueSubmit fail");
		}
	}

	void Queue::Present(const FrameData& frame, Swapchain& swapchain, std::uint32_t swapchainImageIndex)
	{
		auto vkRenderSemaphore = frame._renderSemaphore.Get();
		auto vkSwapchain = swapchain.Get();

		VkPresentInfoKHR present = {};
		present.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
		present.pNext = nullptr;
		present.pSwapchains = &vkSwapchain;
		present.swapchainCount = 1;
		present.pWaitSemaphores = &vkRenderSemaphore;
		present.waitSemaphoreCount = 1;
		present.pImageIndices = &swapchainImageIndex;

		if (vkQueuePresentKHR(_queue, &present) != VK_SUCCESS)
		{
			throw std::runtime_error("vkQueuePresentKHR fail");
		}
	}

	VkQueue* Queue::Get()
	{
		assert(_queue != VK_NULL_HANDLE);
		return &_queue;
	}
}

