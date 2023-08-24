//
// Created by arthur on 25/10/2022.
//
#include <cassert>
#include <stdexcept>

#include "Concerto/Graphics/Vulkan/Wrapper/Device.hpp"
#include "Concerto/Graphics/Vulkan/Wrapper/PhysicalDevice.hpp"
#include "Concerto/Graphics/Vulkan/Wrapper/Instance.hpp"

namespace Concerto::Graphics
{
	std::vector<const char*> deviceExtensions = { VK_KHR_SWAPCHAIN_EXTENSION_NAME };
	Device::Device(PhysicalDevice& physicalDevice, Instance& instance) :
		_physicalDevice(&physicalDevice),
		_device(VK_NULL_HANDLE),
		_allocator(nullptr)
	{
		std::span<VkQueueFamilyProperties> queueFamilyProperties = _physicalDevice->GetQueueFamilyProperties();
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
		createInfo.queueCreateInfoCount = queueCreateInfos.size();
		createInfo.pEnabledFeatures = &deviceFeatures;
		createInfo.pNext = &shader_draw_parameters_features;
		createInfo.enabledExtensionCount = deviceExtensions.size();
		createInfo.ppEnabledExtensionNames = deviceExtensions.data();
		if (vkCreateDevice(*_physicalDevice->Get(), &createInfo, nullptr, &_device) != VK_SUCCESS)
			throw std::runtime_error("failed to create logical device!");
		CreateAllocator(instance);
		_uploadContext = std::make_unique<UploadContext>(*this, GetQueueFamilyIndex(Queue::Type::Graphics));
	}

	UInt32 Device::GetQueueFamilyIndex(Queue::Type queueType)
	{
		std::span<VkQueueFamilyProperties> queueFamilyProperties = _physicalDevice->GetQueueFamilyProperties();
		UInt32 i = 0;
		for (VkQueueFamilyProperties properties: queueFamilyProperties)
		{
			if (properties.queueFlags & VK_QUEUE_GRAPHICS_BIT && queueType == Queue::Type::Graphics)
				return i;
			else if (properties.queueFlags & VK_QUEUE_COMPUTE_BIT && queueType == Queue::Type::Compute)
				return i;
			else if (properties.queueFlags & VK_QUEUE_TRANSFER_BIT && queueType == Queue::Type::Transfer)
				return i;
			i++;
		}
		throw std::runtime_error("No queue family found");
	}

	UInt32 Device::GetQueueFamilyIndex(UInt32 flag)
	{
		std::span<VkQueueFamilyProperties> queueFamilyProperties = _physicalDevice->GetQueueFamilyProperties();
		UInt32 i = 0;
		for (VkQueueFamilyProperties properties: queueFamilyProperties)
		{
			if (properties.queueFlags == flag && flag & VK_QUEUE_GRAPHICS_BIT)
				return i;
			else if (properties.queueFlags == flag && flag & VK_QUEUE_COMPUTE_BIT)
				return i;
			else if (properties.queueFlags == flag && flag & VK_QUEUE_TRANSFER_BIT)
				return i;
			++i;
		}
		throw std::runtime_error("No queue family found");
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
		assert(_device != VK_NULL_HANDLE);
		return &_device;
	}

	void Device::WaitIdle() const
	{
		auto res = vkDeviceWaitIdle(_device);
		if (res != VK_SUCCESS)
			throw std::runtime_error("Failed to Wait for device idle" + std::to_string(res));
	}

	PhysicalDevice& Device::GetPhysicalDevice()
	{
		return *_physicalDevice;
	}

	Allocator& Device::GetAllocator()
	{
		CONCERTO_ASSERT(_allocator != nullptr)
		return *_allocator;
	}

	UploadContext& Device::GetUploadContext()
	{
		return *_uploadContext;
	}

	void Device::CreateAllocator(Instance& instance)
	{
		_allocator = std::make_unique<Allocator>(*_physicalDevice, *this, instance);
		CONCERTO_ASSERT(_allocator != nullptr)
	}

} // Concerto::Graphics::Wrapper