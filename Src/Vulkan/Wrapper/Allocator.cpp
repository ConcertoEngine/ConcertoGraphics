//
// Created by arthur on 21/06/2022.
//
#include <cassert>
#include <stdexcept>
#define VMA_IMPLEMENTATION
#define VMA_STATIC_VULKAN_FUNCTIONS 0
#include "Concerto/Graphics/Vulkan/Wrapper/Allocator.hpp"

#include <Concerto/Core/Assert.hpp>

#include "Concerto/Graphics/Vulkan/Wrapper/Instance.hpp"
#include "Concerto/Graphics/Vulkan/Wrapper/PhysicalDevice.hpp"
#include "Concerto/Graphics/Vulkan/Wrapper/Device.hpp"

namespace Concerto::Graphics
{
	Allocator::Allocator(PhysicalDevice& physicalDevice, Device& device, Instance& instance) :
		Object<VmaAllocator>(device)
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

		_lastResult = vmaCreateAllocator(&allocatorInfo, &_handle);
		CONCERTO_ASSERT(_lastResult == VK_SUCCESS, "ConcertoGraphics: Unable to create vma allocator VkResult={}", static_cast<int>(_lastResult));
	}

	Allocator::~Allocator()
	{
		if (IsNull())
			return;
		vmaDestroyAllocator(_handle);
	}

	Device& Allocator::GetDevice() const
	{
		CONCERTO_ASSERT(_device != nullptr, "ConcertoGraphics: device is null");
		return *_device;
	}

	void Allocator::SetDevice(Device* device)
	{
		_device = device;
	}
}