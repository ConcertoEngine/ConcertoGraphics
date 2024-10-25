//
// Created by arthur on 23/10/2024.
//

#if !defined(CONCERTO_VULKAN_BACKEND_DEVICE_FUNCTION)
	#error You must define CONCERTO_VULKAN_BACKEND_DEVICE_FUNCTION
#endif

#if !defined(CONCERTO_VULKAN_BACKEND_DEVICE_EXT_FUNCTION)
#error You must define CONCERTO_VULKAN_BACKEND_DEVICE_EXT_FUNCTION
#endif

#if !defined(CONCERTO_VULKAN_BACKEND_DEVICE_EXT_BEGIN)
#define CONCERTO_VULKAN_BACKEND_DEVICE_EXT_BEGIN(ext)
#endif

#if !defined(CONCERTO_VULKAN_BACKEND_DEVICE_EXT_END)
#define CONCERTO_VULKAN_BACKEND_DEVICE_EXT_END
#endif

// Vk 1.0
CONCERTO_VULKAN_BACKEND_DEVICE_FUNCTION(vkAllocateCommandBuffers)
CONCERTO_VULKAN_BACKEND_DEVICE_FUNCTION(vkAllocateDescriptorSets)
CONCERTO_VULKAN_BACKEND_DEVICE_FUNCTION(vkAllocateMemory)
CONCERTO_VULKAN_BACKEND_DEVICE_FUNCTION(vkBeginCommandBuffer)
CONCERTO_VULKAN_BACKEND_DEVICE_FUNCTION(vkBindBufferMemory)
CONCERTO_VULKAN_BACKEND_DEVICE_FUNCTION(vkBindImageMemory)
CONCERTO_VULKAN_BACKEND_DEVICE_FUNCTION(vkCmdBeginQuery)
CONCERTO_VULKAN_BACKEND_DEVICE_FUNCTION(vkCmdBeginRenderPass)
CONCERTO_VULKAN_BACKEND_DEVICE_FUNCTION(vkCmdBindDescriptorSets)
CONCERTO_VULKAN_BACKEND_DEVICE_FUNCTION(vkCmdBindIndexBuffer)
CONCERTO_VULKAN_BACKEND_DEVICE_FUNCTION(vkCmdBindPipeline)
CONCERTO_VULKAN_BACKEND_DEVICE_FUNCTION(vkCmdBindVertexBuffers)
CONCERTO_VULKAN_BACKEND_DEVICE_FUNCTION(vkCmdBlitImage)
CONCERTO_VULKAN_BACKEND_DEVICE_FUNCTION(vkCmdClearAttachments)
CONCERTO_VULKAN_BACKEND_DEVICE_FUNCTION(vkCmdClearColorImage)
CONCERTO_VULKAN_BACKEND_DEVICE_FUNCTION(vkCmdClearDepthStencilImage)
CONCERTO_VULKAN_BACKEND_DEVICE_FUNCTION(vkCmdCopyBuffer)
CONCERTO_VULKAN_BACKEND_DEVICE_FUNCTION(vkCmdCopyBufferToImage)
CONCERTO_VULKAN_BACKEND_DEVICE_FUNCTION(vkCmdCopyImage)
CONCERTO_VULKAN_BACKEND_DEVICE_FUNCTION(vkCmdCopyImageToBuffer)
CONCERTO_VULKAN_BACKEND_DEVICE_FUNCTION(vkCmdCopyQueryPoolResults)
CONCERTO_VULKAN_BACKEND_DEVICE_FUNCTION(vkCmdDispatch)
CONCERTO_VULKAN_BACKEND_DEVICE_FUNCTION(vkCmdDispatchIndirect)
CONCERTO_VULKAN_BACKEND_DEVICE_FUNCTION(vkCmdDraw)
CONCERTO_VULKAN_BACKEND_DEVICE_FUNCTION(vkCmdDrawIndexed)
CONCERTO_VULKAN_BACKEND_DEVICE_FUNCTION(vkCmdDrawIndexedIndirect)
CONCERTO_VULKAN_BACKEND_DEVICE_FUNCTION(vkCmdDrawIndirect)
CONCERTO_VULKAN_BACKEND_DEVICE_FUNCTION(vkCmdEndQuery)
CONCERTO_VULKAN_BACKEND_DEVICE_FUNCTION(vkCmdEndRenderPass)
CONCERTO_VULKAN_BACKEND_DEVICE_FUNCTION(vkCmdExecuteCommands)
CONCERTO_VULKAN_BACKEND_DEVICE_FUNCTION(vkCmdFillBuffer)
CONCERTO_VULKAN_BACKEND_DEVICE_FUNCTION(vkCmdNextSubpass)
CONCERTO_VULKAN_BACKEND_DEVICE_FUNCTION(vkCmdPipelineBarrier)
CONCERTO_VULKAN_BACKEND_DEVICE_FUNCTION(vkCmdPushConstants)
CONCERTO_VULKAN_BACKEND_DEVICE_FUNCTION(vkCmdResetEvent)
CONCERTO_VULKAN_BACKEND_DEVICE_FUNCTION(vkCmdResetQueryPool)
CONCERTO_VULKAN_BACKEND_DEVICE_FUNCTION(vkCmdResolveImage)
CONCERTO_VULKAN_BACKEND_DEVICE_FUNCTION(vkCmdSetBlendConstants)
CONCERTO_VULKAN_BACKEND_DEVICE_FUNCTION(vkCmdSetDepthBias)
CONCERTO_VULKAN_BACKEND_DEVICE_FUNCTION(vkCmdSetDepthBounds)
CONCERTO_VULKAN_BACKEND_DEVICE_FUNCTION(vkCmdSetEvent)
CONCERTO_VULKAN_BACKEND_DEVICE_FUNCTION(vkCmdSetLineWidth)
CONCERTO_VULKAN_BACKEND_DEVICE_FUNCTION(vkCmdSetScissor)
CONCERTO_VULKAN_BACKEND_DEVICE_FUNCTION(vkCmdSetStencilCompareMask)
CONCERTO_VULKAN_BACKEND_DEVICE_FUNCTION(vkCmdSetStencilReference)
CONCERTO_VULKAN_BACKEND_DEVICE_FUNCTION(vkCmdSetStencilWriteMask)
CONCERTO_VULKAN_BACKEND_DEVICE_FUNCTION(vkCmdSetViewport)
CONCERTO_VULKAN_BACKEND_DEVICE_FUNCTION(vkCmdUpdateBuffer)
CONCERTO_VULKAN_BACKEND_DEVICE_FUNCTION(vkCmdWaitEvents)
CONCERTO_VULKAN_BACKEND_DEVICE_FUNCTION(vkCmdWriteTimestamp)
CONCERTO_VULKAN_BACKEND_DEVICE_FUNCTION(vkCreateBuffer)
CONCERTO_VULKAN_BACKEND_DEVICE_FUNCTION(vkCreateBufferView)
CONCERTO_VULKAN_BACKEND_DEVICE_FUNCTION(vkCreateCommandPool)
CONCERTO_VULKAN_BACKEND_DEVICE_FUNCTION(vkCreateComputePipelines)
CONCERTO_VULKAN_BACKEND_DEVICE_FUNCTION(vkCreateDescriptorPool)
CONCERTO_VULKAN_BACKEND_DEVICE_FUNCTION(vkCreateDescriptorSetLayout)
CONCERTO_VULKAN_BACKEND_DEVICE_FUNCTION(vkCreateEvent)
CONCERTO_VULKAN_BACKEND_DEVICE_FUNCTION(vkCreateFence)
CONCERTO_VULKAN_BACKEND_DEVICE_FUNCTION(vkCreateFramebuffer)
CONCERTO_VULKAN_BACKEND_DEVICE_FUNCTION(vkCreateGraphicsPipelines)
CONCERTO_VULKAN_BACKEND_DEVICE_FUNCTION(vkCreateImage)
CONCERTO_VULKAN_BACKEND_DEVICE_FUNCTION(vkCreateImageView)
CONCERTO_VULKAN_BACKEND_DEVICE_FUNCTION(vkCreatePipelineCache)
CONCERTO_VULKAN_BACKEND_DEVICE_FUNCTION(vkCreatePipelineLayout)
CONCERTO_VULKAN_BACKEND_DEVICE_FUNCTION(vkCreateQueryPool)
CONCERTO_VULKAN_BACKEND_DEVICE_FUNCTION(vkCreateRenderPass)
CONCERTO_VULKAN_BACKEND_DEVICE_FUNCTION(vkCreateSampler)
CONCERTO_VULKAN_BACKEND_DEVICE_FUNCTION(vkCreateSemaphore)
CONCERTO_VULKAN_BACKEND_DEVICE_FUNCTION(vkCreateShaderModule)
CONCERTO_VULKAN_BACKEND_DEVICE_FUNCTION(vkDestroyBuffer)
CONCERTO_VULKAN_BACKEND_DEVICE_FUNCTION(vkDestroyBufferView)
CONCERTO_VULKAN_BACKEND_DEVICE_FUNCTION(vkDestroyCommandPool)
CONCERTO_VULKAN_BACKEND_DEVICE_FUNCTION(vkDestroyDescriptorPool)
CONCERTO_VULKAN_BACKEND_DEVICE_FUNCTION(vkDestroyDescriptorSetLayout)
CONCERTO_VULKAN_BACKEND_DEVICE_FUNCTION(vkDestroyDevice)
CONCERTO_VULKAN_BACKEND_DEVICE_FUNCTION(vkDestroyEvent)
CONCERTO_VULKAN_BACKEND_DEVICE_FUNCTION(vkDestroyFence)
CONCERTO_VULKAN_BACKEND_DEVICE_FUNCTION(vkDestroyFramebuffer)
CONCERTO_VULKAN_BACKEND_DEVICE_FUNCTION(vkDestroyImage)
CONCERTO_VULKAN_BACKEND_DEVICE_FUNCTION(vkDestroyImageView)
CONCERTO_VULKAN_BACKEND_DEVICE_FUNCTION(vkDestroyPipeline)
CONCERTO_VULKAN_BACKEND_DEVICE_FUNCTION(vkDestroyPipelineCache)
CONCERTO_VULKAN_BACKEND_DEVICE_FUNCTION(vkDestroyPipelineLayout)
CONCERTO_VULKAN_BACKEND_DEVICE_FUNCTION(vkDestroyQueryPool)
CONCERTO_VULKAN_BACKEND_DEVICE_FUNCTION(vkDestroyRenderPass)
CONCERTO_VULKAN_BACKEND_DEVICE_FUNCTION(vkDestroySampler)
CONCERTO_VULKAN_BACKEND_DEVICE_FUNCTION(vkDestroySemaphore)
CONCERTO_VULKAN_BACKEND_DEVICE_FUNCTION(vkDestroyShaderModule)
CONCERTO_VULKAN_BACKEND_DEVICE_FUNCTION(vkDeviceWaitIdle)
CONCERTO_VULKAN_BACKEND_DEVICE_FUNCTION(vkEndCommandBuffer)
CONCERTO_VULKAN_BACKEND_DEVICE_FUNCTION(vkFlushMappedMemoryRanges)
CONCERTO_VULKAN_BACKEND_DEVICE_FUNCTION(vkFreeCommandBuffers)
CONCERTO_VULKAN_BACKEND_DEVICE_FUNCTION(vkFreeDescriptorSets)
CONCERTO_VULKAN_BACKEND_DEVICE_FUNCTION(vkFreeMemory)
CONCERTO_VULKAN_BACKEND_DEVICE_FUNCTION(vkGetBufferMemoryRequirements)
CONCERTO_VULKAN_BACKEND_DEVICE_FUNCTION(vkGetDeviceMemoryCommitment)
CONCERTO_VULKAN_BACKEND_DEVICE_FUNCTION(vkGetDeviceQueue)
CONCERTO_VULKAN_BACKEND_DEVICE_FUNCTION(vkGetEventStatus)
CONCERTO_VULKAN_BACKEND_DEVICE_FUNCTION(vkGetFenceStatus)
CONCERTO_VULKAN_BACKEND_DEVICE_FUNCTION(vkGetImageMemoryRequirements)
CONCERTO_VULKAN_BACKEND_DEVICE_FUNCTION(vkGetImageSparseMemoryRequirements)
CONCERTO_VULKAN_BACKEND_DEVICE_FUNCTION(vkGetImageSubresourceLayout)
CONCERTO_VULKAN_BACKEND_DEVICE_FUNCTION(vkGetPipelineCacheData)
CONCERTO_VULKAN_BACKEND_DEVICE_FUNCTION(vkGetQueryPoolResults)
CONCERTO_VULKAN_BACKEND_DEVICE_FUNCTION(vkGetRenderAreaGranularity)
CONCERTO_VULKAN_BACKEND_DEVICE_FUNCTION(vkInvalidateMappedMemoryRanges)
CONCERTO_VULKAN_BACKEND_DEVICE_FUNCTION(vkMapMemory)
CONCERTO_VULKAN_BACKEND_DEVICE_FUNCTION(vkMergePipelineCaches)
CONCERTO_VULKAN_BACKEND_DEVICE_FUNCTION(vkQueueBindSparse)
CONCERTO_VULKAN_BACKEND_DEVICE_FUNCTION(vkQueueSubmit)
CONCERTO_VULKAN_BACKEND_DEVICE_FUNCTION(vkQueueWaitIdle)
CONCERTO_VULKAN_BACKEND_DEVICE_FUNCTION(vkResetCommandBuffer)
CONCERTO_VULKAN_BACKEND_DEVICE_FUNCTION(vkResetCommandPool)
CONCERTO_VULKAN_BACKEND_DEVICE_FUNCTION(vkResetDescriptorPool)
CONCERTO_VULKAN_BACKEND_DEVICE_FUNCTION(vkResetEvent)
CONCERTO_VULKAN_BACKEND_DEVICE_FUNCTION(vkResetFences)
CONCERTO_VULKAN_BACKEND_DEVICE_FUNCTION(vkSetEvent)
CONCERTO_VULKAN_BACKEND_DEVICE_FUNCTION(vkUnmapMemory)
CONCERTO_VULKAN_BACKEND_DEVICE_FUNCTION(vkUpdateDescriptorSets)
CONCERTO_VULKAN_BACKEND_DEVICE_FUNCTION(vkWaitForFences)

//Vk 1.1
CONCERTO_VULKAN_BACKEND_DEVICE_FUNCTION(vkBindBufferMemory2)
CONCERTO_VULKAN_BACKEND_DEVICE_FUNCTION(vkBindImageMemory2)
CONCERTO_VULKAN_BACKEND_DEVICE_FUNCTION(vkCmdDispatchBase)
CONCERTO_VULKAN_BACKEND_DEVICE_FUNCTION(vkCmdSetDeviceMask)
CONCERTO_VULKAN_BACKEND_DEVICE_FUNCTION(vkCreateDescriptorUpdateTemplate)
CONCERTO_VULKAN_BACKEND_DEVICE_FUNCTION(vkCreateSamplerYcbcrConversion)
CONCERTO_VULKAN_BACKEND_DEVICE_FUNCTION(vkDestroyDescriptorUpdateTemplate)
CONCERTO_VULKAN_BACKEND_DEVICE_FUNCTION(vkDestroySamplerYcbcrConversion)
CONCERTO_VULKAN_BACKEND_DEVICE_FUNCTION(vkGetBufferMemoryRequirements2)
CONCERTO_VULKAN_BACKEND_DEVICE_FUNCTION(vkGetDescriptorSetLayoutSupport)
CONCERTO_VULKAN_BACKEND_DEVICE_FUNCTION(vkGetDeviceGroupPeerMemoryFeatures)
CONCERTO_VULKAN_BACKEND_DEVICE_FUNCTION(vkGetDeviceQueue2)
CONCERTO_VULKAN_BACKEND_DEVICE_FUNCTION(vkGetImageMemoryRequirements2)
CONCERTO_VULKAN_BACKEND_DEVICE_FUNCTION(vkGetImageSparseMemoryRequirements2)
CONCERTO_VULKAN_BACKEND_DEVICE_FUNCTION(vkTrimCommandPool)
CONCERTO_VULKAN_BACKEND_DEVICE_FUNCTION(vkUpdateDescriptorSetWithTemplate)

//Vk 1.2
CONCERTO_VULKAN_BACKEND_DEVICE_FUNCTION(vkCmdBeginRenderPass2)
CONCERTO_VULKAN_BACKEND_DEVICE_FUNCTION(vkCmdDrawIndexedIndirectCount)
CONCERTO_VULKAN_BACKEND_DEVICE_FUNCTION(vkCmdDrawIndirectCount)
CONCERTO_VULKAN_BACKEND_DEVICE_FUNCTION(vkCmdEndRenderPass2)
CONCERTO_VULKAN_BACKEND_DEVICE_FUNCTION(vkCmdNextSubpass2)
CONCERTO_VULKAN_BACKEND_DEVICE_FUNCTION(vkCreateRenderPass2)
CONCERTO_VULKAN_BACKEND_DEVICE_FUNCTION(vkGetBufferDeviceAddress)
CONCERTO_VULKAN_BACKEND_DEVICE_FUNCTION(vkGetBufferOpaqueCaptureAddress)
CONCERTO_VULKAN_BACKEND_DEVICE_FUNCTION(vkGetDeviceMemoryOpaqueCaptureAddress)
CONCERTO_VULKAN_BACKEND_DEVICE_FUNCTION(vkGetSemaphoreCounterValue)
CONCERTO_VULKAN_BACKEND_DEVICE_FUNCTION(vkResetQueryPool)
CONCERTO_VULKAN_BACKEND_DEVICE_FUNCTION(vkSignalSemaphore)
CONCERTO_VULKAN_BACKEND_DEVICE_FUNCTION(vkWaitSemaphores)

//Vk 1.3
CONCERTO_VULKAN_BACKEND_DEVICE_FUNCTION(vkCmdBeginRendering)
CONCERTO_VULKAN_BACKEND_DEVICE_FUNCTION(vkCmdBindVertexBuffers2)
CONCERTO_VULKAN_BACKEND_DEVICE_FUNCTION(vkCmdBlitImage2)
CONCERTO_VULKAN_BACKEND_DEVICE_FUNCTION(vkCmdCopyBuffer2)
CONCERTO_VULKAN_BACKEND_DEVICE_FUNCTION(vkCmdCopyBufferToImage2)
CONCERTO_VULKAN_BACKEND_DEVICE_FUNCTION(vkCmdCopyImage2)
CONCERTO_VULKAN_BACKEND_DEVICE_FUNCTION(vkCmdCopyImageToBuffer2)
CONCERTO_VULKAN_BACKEND_DEVICE_FUNCTION(vkCmdEndRendering)
CONCERTO_VULKAN_BACKEND_DEVICE_FUNCTION(vkCmdPipelineBarrier2)
CONCERTO_VULKAN_BACKEND_DEVICE_FUNCTION(vkCmdResetEvent2)
CONCERTO_VULKAN_BACKEND_DEVICE_FUNCTION(vkCmdResolveImage2)
CONCERTO_VULKAN_BACKEND_DEVICE_FUNCTION(vkCmdSetCullMode)
CONCERTO_VULKAN_BACKEND_DEVICE_FUNCTION(vkCmdSetDepthBiasEnable)
CONCERTO_VULKAN_BACKEND_DEVICE_FUNCTION(vkCmdSetDepthBoundsTestEnable)
CONCERTO_VULKAN_BACKEND_DEVICE_FUNCTION(vkCmdSetDepthCompareOp)
CONCERTO_VULKAN_BACKEND_DEVICE_FUNCTION(vkCmdSetDepthTestEnable)
CONCERTO_VULKAN_BACKEND_DEVICE_FUNCTION(vkCmdSetDepthWriteEnable)
CONCERTO_VULKAN_BACKEND_DEVICE_FUNCTION(vkCmdSetEvent2)
CONCERTO_VULKAN_BACKEND_DEVICE_FUNCTION(vkCmdSetFrontFace)
CONCERTO_VULKAN_BACKEND_DEVICE_FUNCTION(vkCmdSetPrimitiveRestartEnable)
CONCERTO_VULKAN_BACKEND_DEVICE_FUNCTION(vkCmdSetPrimitiveTopology)
CONCERTO_VULKAN_BACKEND_DEVICE_FUNCTION(vkCmdSetRasterizerDiscardEnable)
CONCERTO_VULKAN_BACKEND_DEVICE_FUNCTION(vkCmdSetScissorWithCount)
CONCERTO_VULKAN_BACKEND_DEVICE_FUNCTION(vkCmdSetStencilOp)
CONCERTO_VULKAN_BACKEND_DEVICE_FUNCTION(vkCmdSetStencilTestEnable)
CONCERTO_VULKAN_BACKEND_DEVICE_FUNCTION(vkCmdSetViewportWithCount)
CONCERTO_VULKAN_BACKEND_DEVICE_FUNCTION(vkCmdWaitEvents2)
CONCERTO_VULKAN_BACKEND_DEVICE_FUNCTION(vkCmdWriteTimestamp2)
CONCERTO_VULKAN_BACKEND_DEVICE_FUNCTION(vkCreatePrivateDataSlot)
CONCERTO_VULKAN_BACKEND_DEVICE_FUNCTION(vkDestroyPrivateDataSlot)
CONCERTO_VULKAN_BACKEND_DEVICE_FUNCTION(vkGetDeviceBufferMemoryRequirements)
CONCERTO_VULKAN_BACKEND_DEVICE_FUNCTION(vkGetDeviceImageMemoryRequirements)
CONCERTO_VULKAN_BACKEND_DEVICE_FUNCTION(vkGetDeviceImageSparseMemoryRequirements)
CONCERTO_VULKAN_BACKEND_DEVICE_FUNCTION(vkGetPrivateData)
CONCERTO_VULKAN_BACKEND_DEVICE_FUNCTION(vkQueueSubmit2)
CONCERTO_VULKAN_BACKEND_DEVICE_FUNCTION(vkSetPrivateData)

CONCERTO_VULKAN_BACKEND_DEVICE_EXT_BEGIN(VK_KHR_swapchain)
	CONCERTO_VULKAN_BACKEND_DEVICE_EXT_FUNCTION(vkAcquireNextImageKHR)
	CONCERTO_VULKAN_BACKEND_DEVICE_EXT_FUNCTION(vkCreateSwapchainKHR)
	CONCERTO_VULKAN_BACKEND_DEVICE_EXT_FUNCTION(vkDestroySwapchainKHR)
	CONCERTO_VULKAN_BACKEND_DEVICE_EXT_FUNCTION(vkGetSwapchainImagesKHR)
	CONCERTO_VULKAN_BACKEND_DEVICE_EXT_FUNCTION(vkQueuePresentKHR)
CONCERTO_VULKAN_BACKEND_DEVICE_EXT_END

CONCERTO_VULKAN_BACKEND_DEVICE_EXT_BEGIN(VK_EXT_debug_marker)
	CONCERTO_VULKAN_BACKEND_DEVICE_EXT_FUNCTION(vkCmdDebugMarkerBeginEXT)
	CONCERTO_VULKAN_BACKEND_DEVICE_EXT_FUNCTION(vkCmdDebugMarkerEndEXT)
	CONCERTO_VULKAN_BACKEND_DEVICE_EXT_FUNCTION(vkCmdDebugMarkerInsertEXT)
	CONCERTO_VULKAN_BACKEND_DEVICE_EXT_FUNCTION(vkDebugMarkerSetObjectNameEXT)
	CONCERTO_VULKAN_BACKEND_DEVICE_EXT_FUNCTION(vkDebugMarkerSetObjectTagEXT)
CONCERTO_VULKAN_BACKEND_DEVICE_EXT_END

#undef CONCERTO_VULKAN_BACKEND_DEVICE_FUNCTION
#undef CONCERTO_VULKAN_BACKEND_DEVICE_EXT_FUNCTION
#undef CONCERTO_VULKAN_BACKEND_DEVICE_EXT_BEGIN
#undef CONCERTO_VULKAN_BACKEND_DEVICE_EXT_END