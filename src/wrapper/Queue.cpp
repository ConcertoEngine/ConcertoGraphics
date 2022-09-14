//
// Created by arthur on 13/09/2022.
//
#include "wrapper/Queue.hpp"

namespace Concerto::Graphics::Wrapper
{
	Queue::Queue(const vkb::Device& device) : _device(device),
											  _queue(_device.get_queue(vkb::QueueType::graphics).value())
	{

	}

	std::uint32_t Queue::GetFamilyIndex() const
	{
		return _device.get_queue_index(vkb::QueueType::graphics).value();
	}

	void Queue::Submit(const FrameData& frame)
	{
		VkPipelineStageFlags waitStage = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
		auto vkPresentSemaphore = frame._presentSemaphore.get();
		auto vkRenderSemaphore = frame._renderSemaphore.get();
		auto vkCommandBuffer = frame._mainCommandBuffer.get();
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
		if (vkQueueSubmit(_queue, 1, &submit, frame._renderFence.get()) != VK_SUCCESS)
		{
			throw std::runtime_error("vkQueueSubmit fail");
		}
	}

	void Queue::Present(const FrameData& frame, Swapchain& swapchain, std::uint32_t swapchainImageIndex)
	{
		auto vkRenderSemaphore = frame._renderSemaphore.get();
		auto vkSwapchain = swapchain.get();

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
}

