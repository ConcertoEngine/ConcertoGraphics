//
// Created by arthur on 25/10/2022.
//

#include <cassert>
#include <stdexcept>

#include <Concerto/Core/Assert.hpp>

#include "Concerto/Graphics/Backend/Vulkan/Wrapper/Device.hpp"
#include "Concerto/Graphics/Backend/Vulkan/Wrapper/PhysicalDevice.hpp"
#include "Concerto/Graphics/Backend/Vulkan/Wrapper/Instance.hpp"

namespace Concerto::Graphics::Vk
{
	std::vector<const char*> deviceExtensions = { VK_KHR_SWAPCHAIN_EXTENSION_NAME, VK_EXT_DEBUG_MARKER_EXTENSION_NAME };
	Device::Device(PhysicalDevice& physicalDevice, Instance& instance) :
		_physicalDevice(physicalDevice),
		_device(VK_NULL_HANDLE),
		_allocator(nullptr),
		_instance(&instance)
	{
		std::span<VkQueueFamilyProperties> queueFamilyProperties = _physicalDevice.GetQueueFamilyProperties();
		std::vector<VkDeviceQueueCreateInfo> queueCreateInfos;
		queueCreateInfos.reserve(queueFamilyProperties.size());

		for (VkQueueFamilyProperties property: queueFamilyProperties)
		{
			auto it = std::find_if(queueCreateInfos.begin(), queueCreateInfos.end(),
					[&](const VkDeviceQueueCreateInfo& queueCreateInfo)
					{
						return queueCreateInfo.queueFamilyIndex == property.queueFlags;
					});
			if (it == queueCreateInfos.end())
			{
				VkDeviceQueueCreateInfo queueCreateInfo{};
				float queuePriority = 1.0f;
				queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
				queueCreateInfo.queueFamilyIndex = GetQueueFamilyIndex(property.queueFlags);
				queueCreateInfo.queueCount = 1;
				queueCreateInfo.flags = 0;
				queueCreateInfo.pQueuePriorities = &queuePriority;
				queueCreateInfos.push_back(queueCreateInfo);
			}
		}

		VkPhysicalDeviceFeatures deviceFeatures{};
		VkPhysicalDeviceShaderDrawParametersFeatures shader_draw_parameters_features = {};
		shader_draw_parameters_features.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_SHADER_DRAW_PARAMETERS_FEATURES;
		shader_draw_parameters_features.pNext = nullptr;
		shader_draw_parameters_features.shaderDrawParameters = VK_TRUE;

		VkDeviceCreateInfo createInfo{};
		createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
		createInfo.pQueueCreateInfos = queueCreateInfos.data();
		createInfo.queueCreateInfoCount = static_cast<UInt32>(queueCreateInfos.size());
		createInfo.pEnabledFeatures = &deviceFeatures;
		createInfo.pNext = &shader_draw_parameters_features;
		createInfo.enabledExtensionCount = static_cast<UInt32>(deviceExtensions.size());
		createInfo.ppEnabledExtensionNames = deviceExtensions.data();
		const VkResult result = vkCreateDevice(*_physicalDevice.Get(), &createInfo, nullptr, &_device);
		CONCERTO_ASSERT(result == VK_SUCCESS, "Error cannot create logical device: VkResult={}", static_cast<int>(result));
		CreateAllocator(instance);
	}

	UInt32 Device::GetQueueFamilyIndex(Queue::Type queueType) const
	{
		const std::span<VkQueueFamilyProperties> queueFamilyProperties = _physicalDevice.GetQueueFamilyProperties();
		UInt32 i = 0;
		for (const VkQueueFamilyProperties& properties: queueFamilyProperties)
		{
			if (properties.queueFlags & VK_QUEUE_GRAPHICS_BIT && queueType == Queue::Type::Graphics)
				return i;
			else if (properties.queueFlags & VK_QUEUE_COMPUTE_BIT && queueType == Queue::Type::Compute)
				return i;
			else if (properties.queueFlags & VK_QUEUE_TRANSFER_BIT && queueType == Queue::Type::Transfer)
				return i;
			i++;
		}
		CONCERTO_ASSERT_FALSE("No queue family found");
		return std::numeric_limits<UInt32>::max();
	}

	UInt32 Device::GetQueueFamilyIndex(UInt32 flag) const
	{
		const std::span<VkQueueFamilyProperties> queueFamilyProperties = _physicalDevice.GetQueueFamilyProperties();
		UInt32 i = 0;
		for (const VkQueueFamilyProperties properties: queueFamilyProperties)
		{
			if (properties.queueFlags == flag && flag & VK_QUEUE_GRAPHICS_BIT)
				return i;
			if (properties.queueFlags == flag && flag & VK_QUEUE_COMPUTE_BIT)
				return i;
			if (properties.queueFlags == flag && flag & VK_QUEUE_TRANSFER_BIT)
				return i;
			++i;
		}
		CONCERTO_ASSERT_FALSE("No queue family found");;
		return std::numeric_limits<UInt32>::max();
	}

	Queue& Device::GetQueue(Queue::Type queueType)
	{
		auto it = _queues.find(queueType);
		if (it != _queues.end())
			return it->second;
		auto emplace = _queues.emplace(queueType, Queue(*this, GetQueueFamilyIndex(queueType)));
		return emplace.first->second;
	}

	VkDevice* Device::Get()
	{
		CONCERTO_ASSERT(_device != VK_NULL_HANDLE, "ConcertoGraphics: device handle is null");
		return &_device;
	}

	void Device::WaitIdle() const
	{
		const VkResult result = vkDeviceWaitIdle(_device);
		CONCERTO_ASSERT(result == VK_SUCCESS, "ConcertoGraphics: Failed to Wait for device idle VkResult={}", static_cast<int>(result));
	}

	void Device::UpdateDescriptorSetsWrite(std::span<VkWriteDescriptorSet> descriptorWrites) const
	{
		vkUpdateDescriptorSets(_device, static_cast<UInt32>(descriptorWrites.size()), descriptorWrites.data(), 0, nullptr);
	}

	void Device::UpdateDescriptorSetWrite(VkWriteDescriptorSet descriptorWrite)
	{
		vkUpdateDescriptorSets(_device, 1, &descriptorWrite, 0, nullptr);
	}

	void Device::SetObjectName(UInt64 object, std::string_view name)
	{
		//auto pfnDebugMarkerSetObjectTag = (PFN_vkDebugMarkerSetObjectTagEXT)vkGetDeviceProcAddr(_device, "vkDebugMarkerSetObjectTagEXT");
		auto pfnDebugMarkerSetObjectName = (PFN_vkDebugMarkerSetObjectNameEXT)vkGetDeviceProcAddr(_device, "vkDebugMarkerSetObjectNameEXT");
		//auto pfnCmdDebugMarkerBegin = (PFN_vkCmdDebugMarkerBeginEXT)vkGetDeviceProcAddr(_device, "vkCmdDebugMarkerBeginEXT");
		//auto pfnCmdDebugMarkerEnd = (PFN_vkCmdDebugMarkerEndEXT)vkGetDeviceProcAddr(_device, "vkCmdDebugMarkerEndEXT");
		//auto pfnCmdDebugMarkerInsert = (PFN_vkCmdDebugMarkerInsertEXT)vkGetDeviceProcAddr(_device, "vkCmdDebugMarkerInsertEXT");

		VkDebugMarkerObjectNameInfoEXT nameInfo = {};
		nameInfo.sType = VK_STRUCTURE_TYPE_DEBUG_MARKER_OBJECT_NAME_INFO_EXT;
		nameInfo.objectType = VK_DEBUG_REPORT_OBJECT_TYPE_UNKNOWN_EXT;
		nameInfo.object = object;
		nameInfo.pObjectName = name.data();
		if (pfnDebugMarkerSetObjectName)
			pfnDebugMarkerSetObjectName(_device, &nameInfo);
	}

	PhysicalDevice& Device::GetPhysicalDevice() const
	{
		return _physicalDevice;
	}

	Allocator& Device::GetAllocator() const
	{
		CONCERTO_ASSERT(_allocator != nullptr, "ConcertoGraphics: Allocator handle is null");
		return *_allocator;
	}

	Instance& Device::GetInstance()
	{
		CONCERTO_ASSERT(_instance, "ConcertoGraphics: Invalid Vulkan instance.");
		return *_instance;
	}

	void Device::CreateAllocator(Instance& instance)
	{
		_allocator = std::make_unique<Allocator>(_physicalDevice, *this, instance);
		CONCERTO_ASSERT(_allocator != nullptr, "ConcertoGraphics: Cannot create allocator");
	}

} // Concerto::Graphics::Vk