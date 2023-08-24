//
// Created by arthur on 21/06/2022.
//
#include <cassert>
#include <stdexcept>
#define VMA_IMPLEMENTATION
#define VMA_STATIC_VULKAN_FUNCTIONS 0
#include "Concerto/Graphics/Vulkan/wrapper/Allocator.hpp"
#include "Concerto/Graphics/Vulkan/wrapper/Instance.hpp"
#include "Concerto/Graphics/Vulkan/wrapper/PhysicalDevice.hpp"
#include "Concerto/Graphics/Vulkan/wrapper/Device.hpp"

namespace Concerto::Graphics
{
	Allocator::Allocator(PhysicalDevice& physicalDevice, Device& device, Instance& instance) :
		Object<VmaAllocator>(device, [](Device &device, VmaAllocator handle)
			{
				vmaDestroyAllocator(handle);
			}),
		_device(&device)
	{
		VmaVulkanFunctions vulkanFunctions {
			.vkGetInstanceProcAddr = vkGetInstanceProcAddr,
			.vkGetDeviceProcAddr = vkGetDeviceProcAddr,
			.vkGetPhysicalDeviceProperties = vkGetPhysicalDeviceProperties,
			.vkGetPhysicalDeviceMemoryProperties = vkGetPhysicalDeviceMemoryProperties,
			.vkAllocateMemory = vkAllocateMemory,
			.vkFreeMemory = vkFreeMemory,
			.vkMapMemory = vkMapMemory,
			.vkUnmapMemory = vkUnmapMemory,
			.vkFlushMappedMemoryRanges = vkFlushMappedMemoryRanges,
			.vkInvalidateMappedMemoryRanges = vkInvalidateMappedMemoryRanges,
			.vkBindBufferMemory = vkBindBufferMemory,
			.vkBindImageMemory = vkBindImageMemory,
			.vkGetBufferMemoryRequirements = vkGetBufferMemoryRequirements,
			.vkGetImageMemoryRequirements = vkGetImageMemoryRequirements,
			.vkCreateBuffer = vkCreateBuffer,
			.vkDestroyBuffer = vkDestroyBuffer,
			.vkCreateImage = vkCreateImage,
			.vkDestroyImage = vkDestroyImage,
			.vkCmdCopyBuffer = vkCmdCopyBuffer
		};
		VmaAllocatorCreateInfo allocatorInfo = {};
		allocatorInfo.physicalDevice = *physicalDevice.Get();
		allocatorInfo.device = *device.Get();
		allocatorInfo.instance = *instance.Get();
		allocatorInfo.pVulkanFunctions = &vulkanFunctions;
		
		if (vmaCreateAllocator(&allocatorInfo, &_handle) != VK_SUCCESS)
		{
			throw std::runtime_error("VMA : Unable to create allocator");
		}
	}

	Device& Allocator::GetDevice() const
	{
		assert(_device != nullptr);
		return *_device;
	}
}