//
// Created by arthur on 13/09/2022.
//

#include "Concerto/Graphics/Backend/Vulkan/Wrapper/Queue.hpp"
#include "Concerto/Graphics/Backend/Vulkan/Wrapper/RenderPass.hpp"
#include "Concerto/Graphics/Backend/Vulkan/Wrapper/Swapchain.hpp"
#include "Concerto/Graphics/Backend/Vulkan/Wrapper/CommandBuffer.hpp"
#include "Concerto/Graphics/Backend/Vulkan/Wrapper/Semaphore.hpp"
#include "Concerto/Graphics/Backend/Vulkan/Wrapper/Fence.hpp"
#include "Concerto/Graphics/Backend/Vulkan/Wrapper/Device.hpp"
#include "Concerto/Graphics/Frame.hpp"

namespace Concerto::Graphics::Vk
{
	Queue::Queue(Device& device, UInt32 queueFamilyIndex) :
		Object<VkQueue>(device),
		_queueFamilyIndex(queueFamilyIndex)
	{
		_device->vkGetDeviceQueue(*_device->Get(), _queueFamilyIndex, 0, &_handle);
	}

	UInt32 Queue::GetFamilyIndex() const
	{
		return _queueFamilyIndex;
	}

	void Queue::Submit(const CommandBuffer& commandBuffer, const Semaphore* presentSemaphore, const Semaphore* renderSemaphore, const Fence& renderFence) const
	{
		constexpr VkPipelineStageFlags waitStage = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
		VkSubmitInfo submit = {};
		submit.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
		submit.pNext = nullptr;
		submit.pWaitDstStageMask = &waitStage;
		submit.waitSemaphoreCount = presentSemaphore ? 1 : 0;
		submit.pWaitSemaphores = presentSemaphore ? presentSemaphore->Get() : VK_NULL_HANDLE,
		submit.signalSemaphoreCount = renderSemaphore ? 1 : 0;
		submit.pSignalSemaphores = renderSemaphore ? renderSemaphore->Get() : VK_NULL_HANDLE;
		submit.commandBufferCount = 1;
		submit.pCommandBuffers = commandBuffer.Get();
		_lastResult = _device->vkQueueSubmit(_handle, 1, &submit, *renderFence.Get());
		CONCERTO_ASSERT(_lastResult == VK_SUCCESS, "ConcertoGraphics: vkQueueSubmit failed VKResult={}", static_cast<int>(_lastResult));
	}

	bool Queue::Present(const Semaphore& renderSemaphore, SwapChain& swapchain, UInt32 swapchainImageIndex) const
	{
		VkPresentInfoKHR present = {};
		present.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
		present.pNext = nullptr;
		present.pSwapchains = swapchain.Get();
		present.swapchainCount = 1;
		present.pWaitSemaphores = renderSemaphore.Get();
		present.waitSemaphoreCount = 1;
		present.pImageIndices = &swapchainImageIndex;
		_lastResult = _device->vkQueuePresentKHR(_handle, &present);
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

