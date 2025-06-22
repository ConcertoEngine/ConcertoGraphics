//
// Created by arthur on 25/10/2023.
//
#include <Concerto/Core/Assert.hpp>

#include "Concerto/Graphics/Backend/Vulkan/Wrapper/ObjectDebug.hpp"
#include "Concerto/Graphics/Backend/Vulkan/Wrapper/Device.hpp"
#ifdef CCT_DEBUG

namespace
{
	VkDebugReportObjectTypeEXT TypeNameToVkDebugReportObjectTypeEXT(std::string_view typeName)
	{
		using namespace std::string_view_literals;
		if (typeName == "VkInstance_T"sv)
			return VK_DEBUG_REPORT_OBJECT_TYPE_INSTANCE_EXT;
		if (typeName == "VkPhysicalDevice_T"sv)
			return VK_DEBUG_REPORT_OBJECT_TYPE_PHYSICAL_DEVICE_EXT;
		if (typeName == "VkDevice_T"sv)
			return VK_DEBUG_REPORT_OBJECT_TYPE_DEVICE_EXT;
		if (typeName == "VkQueue_T"sv)
			return VK_DEBUG_REPORT_OBJECT_TYPE_QUEUE_EXT;
		if (typeName == "VkSemaphore_T"sv)
			return VK_DEBUG_REPORT_OBJECT_TYPE_SEMAPHORE_EXT;
		if (typeName == "VkCommandBuffer_T"sv)
			return VK_DEBUG_REPORT_OBJECT_TYPE_COMMAND_BUFFER_EXT;
		if (typeName == "VkFence_T"sv)
			return VK_DEBUG_REPORT_OBJECT_TYPE_FENCE_EXT;
		if (typeName == "VkDeviceMemory_T"sv)
			return VK_DEBUG_REPORT_OBJECT_TYPE_DEVICE_MEMORY_EXT;
		if (typeName == "VkBuffer_T"sv)
			return VK_DEBUG_REPORT_OBJECT_TYPE_BUFFER_EXT;
		if (typeName == "VkImage_T"sv)
			return VK_DEBUG_REPORT_OBJECT_TYPE_IMAGE_EXT;
		if (typeName == "VkEvent_T"sv)
			return VK_DEBUG_REPORT_OBJECT_TYPE_EVENT_EXT;
		if (typeName == "VkQueryPool_T"sv)
			return VK_DEBUG_REPORT_OBJECT_TYPE_QUERY_POOL_EXT;
		if (typeName == "VkBufferView_T"sv)
			return VK_DEBUG_REPORT_OBJECT_TYPE_BUFFER_VIEW_EXT;
		if (typeName == "VkImageView_T"sv)
			return VK_DEBUG_REPORT_OBJECT_TYPE_IMAGE_VIEW_EXT;
		if (typeName == "VkShaderModule_T"sv)
			return VK_DEBUG_REPORT_OBJECT_TYPE_SHADER_MODULE_EXT;
		if (typeName == "VPipelineCachek_T"sv)
			return VK_DEBUG_REPORT_OBJECT_TYPE_PIPELINE_CACHE_EXT;
		if (typeName == "VkPipelineLayout_T"sv)
			return VK_DEBUG_REPORT_OBJECT_TYPE_PIPELINE_LAYOUT_EXT;
		if (typeName == "VkRenderPass_T"sv)
			return VK_DEBUG_REPORT_OBJECT_TYPE_RENDER_PASS_EXT;
		if (typeName == "VkPipeline_T"sv)
			return VK_DEBUG_REPORT_OBJECT_TYPE_PIPELINE_EXT;
		if (typeName == "VkDescriptorSetLayout_T"sv)
			return VK_DEBUG_REPORT_OBJECT_TYPE_DESCRIPTOR_SET_LAYOUT_EXT;
		if (typeName == "VkSampler_T"sv)
			return VK_DEBUG_REPORT_OBJECT_TYPE_SAMPLER_EXT;
		if (typeName == "VkDescriptorPool_T"sv)
			return VK_DEBUG_REPORT_OBJECT_TYPE_DESCRIPTOR_POOL_EXT;
		if (typeName == "VkDescriptorSet_T"sv)
			return VK_DEBUG_REPORT_OBJECT_TYPE_DESCRIPTOR_SET_EXT;
		if (typeName == "VkFrameBuffer_T"sv)
			return VK_DEBUG_REPORT_OBJECT_TYPE_FRAMEBUFFER_EXT;
		if (typeName == "VkCommandPool_T"sv)
			return VK_DEBUG_REPORT_OBJECT_TYPE_COMMAND_POOL_EXT;
		if (typeName == "VkSurfaceKHR_T"sv)
			return VK_DEBUG_REPORT_OBJECT_TYPE_SURFACE_KHR_EXT;
		if (typeName == "VkSwapchainKHR_T"sv)
			return VK_DEBUG_REPORT_OBJECT_TYPE_SWAPCHAIN_KHR_EXT;
		if (typeName == "VkDebugReportCallbackEXT_T"sv)
			return VK_DEBUG_REPORT_OBJECT_TYPE_DEBUG_REPORT_CALLBACK_EXT_EXT;
		if (typeName == "VkDisplayKHR_T"sv)
			return VK_DEBUG_REPORT_OBJECT_TYPE_DISPLAY_KHR_EXT;
		if (typeName == "VkDisplayModeKHR_T"sv)
			return VK_DEBUG_REPORT_OBJECT_TYPE_DISPLAY_MODE_KHR_EXT;
		if (typeName == "VkValidationCacheEXT_T"sv)
			return VK_DEBUG_REPORT_OBJECT_TYPE_VALIDATION_CACHE_EXT_EXT;
		return VK_DEBUG_REPORT_OBJECT_TYPE_UNKNOWN_EXT;
	}
}

namespace cct::gfx::vk
{
	ObjectDebug::ObjectDebug(Device& device, std::string_view typeName, void** vkHandle) :
		_device(&device),
		_typeName(typeName),
		_debugReportObjectType(TypeNameToVkDebugReportObjectTypeEXT(typeName)),
		_vkHandle(vkHandle)
	{
	}

	VkDebugReportObjectTypeEXT ObjectDebug::GetDebugReportObjectType() const
	{
		if (!_device->IsExtensionEnabled(VK_EXT_DEBUG_REPORT_EXTENSION_NAME))
		{
			CCT_ASSERT_FALSE("ObjectDebug::GetDebugReportObjectType() is called but extension " VK_EXT_DEBUG_REPORT_EXTENSION_NAME " is not enabled");
		}
		return _debugReportObjectType;
	}

	UInt64 ObjectDebug::GetObject() const
	{
		return reinterpret_cast<UInt64>(*_vkHandle);
	}

	void ObjectDebug::SetDebugName(std::string_view name)
	{
		if (!_device->IsExtensionEnabled(VK_EXT_DEBUG_MARKER_EXTENSION_NAME))
		{
			CCT_ASSERT_FALSE("ObjectDebug::SetDebugName is called but extension " VK_EXT_DEBUG_MARKER_EXTENSION_NAME " is not enabled");
			return;
		}
		_debugName = name;
		VkDebugMarkerObjectNameInfoEXT nameInfo = {};
		nameInfo.sType = VK_STRUCTURE_TYPE_DEBUG_MARKER_OBJECT_NAME_INFO_EXT;
		nameInfo.objectType = _debugReportObjectType;
		nameInfo.object = GetObject();
		nameInfo.pObjectName = name.data();
		_device->vkDebugMarkerSetObjectNameEXT(*_device->Get(), &nameInfo);
	}

	std::string_view ObjectDebug::GetDebugName() const
	{
		CCT_ASSERT(!_debugName.empty(), "ConcertoGraphics: ObjectDebug::GetDebugName() has been called but ObjectDebug::SetDebugName was never called");
		return _debugName;
	}

	ObjectDebug::ObjectDebug(ObjectDebug&& other) noexcept
	{
		_device = std::exchange(other._device, VK_NULL_HANDLE);
		_typeName = std::exchange(other._typeName, {});
		_debugReportObjectType = std::exchange(other._debugReportObjectType, VK_DEBUG_REPORT_OBJECT_TYPE_UNKNOWN_EXT);
		_vkHandle = std::exchange(other._vkHandle, VK_NULL_HANDLE);
		_debugName = std::exchange(other._debugName, {});
	}

	ObjectDebug& ObjectDebug::operator=(ObjectDebug&& other)
	{
		std::swap(_device, other._device);
		std::swap(_typeName, other._typeName);
		std::swap(_debugReportObjectType, other._debugReportObjectType);
		std::swap(_vkHandle, other._vkHandle);
		std::swap(_debugName, other._debugName);

		return *this;
	}
}

#endif