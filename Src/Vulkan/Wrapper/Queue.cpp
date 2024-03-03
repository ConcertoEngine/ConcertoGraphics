//
// Created by arthur on 13/09/2022.
//

#include "Concerto/Graphics/Vulkan/Wrapper/Queue.hpp"
#include "Concerto/Graphics/Vulkan/Wrapper/RenderPass.hpp"
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
		_lastResult = vkQueueSubmit(_handle, 1, &submit, *frame.renderFence.Get());
		CONCERTO_ASSERT(_lastResult == VK_SUCCESS, "ConcertoGraphics: vkQueueSubmit failed VKResult={}", static_cast<int>(_lastResult));
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
		_lastResult = vkQueuePresentKHR(_handle, &present);
		if (_lastResult == VK_ERROR_OUT_OF_DATE_KHR || _lastResult == VK_SUBOPTIMAL_KHR)
		{
			return false;
		}
		if (_lastResult != VK_SUCCESS)
		{
			CONCERTO_ASSERT_FALSE("ConcertoGraphics: vkQueuePresentKHR failed VKResult={}", static_cast<int>(_lastResult));
			return false;
		}
		return true;
	}
}

