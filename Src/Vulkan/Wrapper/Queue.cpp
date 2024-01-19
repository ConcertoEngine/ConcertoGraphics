//
// Created by arthur on 13/09/2022.
//

#include "Concerto/Graphics/Vulkan/Wrapper/Queue.hpp"
#include "Concerto/Graphics/Vulkan/Wrapper/Swapchain.hpp"
#include "Concerto/Graphics/Vulkan/Wrapper/CommandBuffer.hpp"
#include "Concerto/Graphics/Vulkan/Wrapper/Device.hpp"
#include "Concerto/Graphics/Frame.hpp"

namespace Concerto::Graphics
{
	Queue::Queue(Device& device, UInt32 queueFamilyIndex) : Object<VkQueue>(device), _queueFamilyIndex(queueFamilyIndex)
	{
		vkGetDeviceQueue(*_device->Get(), _queueFamilyIndex, 0, &_handle);
	}

	UInt32 Queue::GetFamilyIndex() const
	{
		return _queueFamilyIndex;
	}

	void Queue::Submit(const FrameData& frame)
	{
		VkCommandBuffer vkCommandBuffer = frame.commandBuffer.Get();
		VkPipelineStageFlags waitStage = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
		VkSubmitInfo submit = {};
		submit.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
		submit.pNext = nullptr;
		submit.pWaitDstStageMask = &waitStage;
		submit.waitSemaphoreCount = 1;
		submit.pWaitSemaphores = frame.presentSemaphore.Get();
		submit.signalSemaphoreCount = 1;
		submit.pSignalSemaphores = frame.renderSemaphore.Get();
		submit.commandBufferCount = 1;
		submit.pCommandBuffers = &vkCommandBuffer;
		if (vkQueueSubmit(_handle, 1, &submit, *frame.renderFence.Get()) != VK_SUCCESS)
		{
			CONCERTO_ASSERT_FALSE;
			throw std::runtime_error("vkQueueSubmit fail");
		}
	}

	bool Queue::Present(const FrameData& frame, Swapchain& swapchain, UInt32 swapchainImageIndex)
	{
		VkPresentInfoKHR present = {};
		present.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
		present.pNext = nullptr;
		present.pSwapchains = swapchain.Get();
		present.swapchainCount = 1;
		present.pWaitSemaphores = frame.renderSemaphore.Get();
		present.waitSemaphoreCount = 1;
		present.pImageIndices = &swapchainImageIndex;
		auto vkResult = vkQueuePresentKHR(_handle, &present);
		if (vkResult == VK_ERROR_OUT_OF_DATE_KHR || vkResult == VK_SUBOPTIMAL_KHR)
		{
			return false;
		}
		else if (vkResult != VK_SUCCESS)
		{
			CONCERTO_ASSERT_FALSE;
			throw std::runtime_error("vkQueuePresentKHR fail");
		}
		return true;
	}
}

