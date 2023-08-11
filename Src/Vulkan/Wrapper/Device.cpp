//
// Created by arthur on 25/10/2022.
//

#include <cassert>
#include <stdexcept>

#include "Concerto/Graphics/Vulkan/Wrapper/Device.hpp"
#include "Concerto/Graphics/Vulkan/Wrapper/PhysicalDevice.hpp"

namespace Concerto::Graphics
{
	Device::Device(PhysicalDevice& physicalDevice, std::span<const char*> extentions) : _physicalDevice(&physicalDevice), _device(VK_NULL_HANDLE)
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
		createInfo.enabledExtensionCount = extentions.size();
		createInfo.ppEnabledExtensionNames = extentions.data();
		if (vkCreateDevice(*_physicalDevice->Get(), &createInfo, nullptr, &_device) != VK_SUCCESS)
			throw std::runtime_error("failed to create logical device!");
	}

	std::uint32_t Device::GetQueueFamilyIndex(Queue::Type queueType)
	{
		std::span<VkQueueFamilyProperties> queueFamilyProperties = _physicalDevice->GetQueueFamilyProperties();
		std::uint32_t i = 0;
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

	std::uint32_t Device::GetQueueFamilyIndex(std::uint32_t flag)
	{
		std::span<VkQueueFamilyProperties> queueFamilyProperties = _physicalDevice->GetQueueFamilyProperties();
		std::uint32_t i = 0;
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

	Queue Device::GetQueue(Queue::Type queueType)
	{
		return Queue(*this, GetQueueFamilyIndex(queueType));
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

} // Concerto::Graphics::Wrapper