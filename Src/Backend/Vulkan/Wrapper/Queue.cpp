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

namespace cct::gfx::vk
{
	Queue::Queue(Device& device, UInt32 queueFamilyIndex) :
		Object<VkQueue>(device),
		m_queueFamilyIndex(queueFamilyIndex)
	{
		m_device->vkGetDeviceQueue(*m_device->Get(), m_queueFamilyIndex, 0, &m_handle);
	}

	UInt32 Queue::GetFamilyIndex() const
	{
		return m_queueFamilyIndex;
	}

	void Queue::Submit(const CommandBuffer& commandBuffer, const Semaphore* presentSemaphore, const Semaphore* renderSemaphore, const Fence& renderFence) const
	{
		CCT_GFX_AUTO_PROFILER_SCOPE();

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
		m_lastResult = m_device->vkQueueSubmit(m_handle, 1, &submit, *renderFence.Get());
		CCT_ASSERT(m_lastResult == VK_SUCCESS, "ConcertoGraphics: vkQueueSubmit failed VKResult={}", static_cast<int>(m_lastResult));
	}

	bool Queue::Present(const Semaphore& renderSemaphore, SwapChain& swapchain, UInt32 swapchainImageIndex) const
	{
		CCT_GFX_AUTO_PROFILER_SCOPE();

		VkPresentInfoKHR present = {};
		present.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
		present.pNext = nullptr;
		present.pSwapchains = swapchain.Get();
		present.swapchainCount = 1;
		present.pWaitSemaphores = renderSemaphore.Get();
		present.waitSemaphoreCount = 1;
		present.pImageIndices = &swapchainImageIndex;
		m_lastResult = m_device->vkQueuePresentKHR(m_handle, &present);
		if (m_lastResult == VK_ERROR_OUT_OF_DATE_KHR || m_lastResult == VK_SUBOPTIMAL_KHR)
		{
			return false;
		}
		if (m_lastResult != VK_SUCCESS)
		{
			CCT_ASSERT_FALSE("ConcertoGraphics: vkQueuePresentKHR failed VKResult={}", static_cast<int>(m_lastResult));
			return false;
		}
		return true;
	}
}

