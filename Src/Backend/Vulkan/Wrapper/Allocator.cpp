//
// Created by arthur on 21/06/2022.
//
#include <cassert>
#include <stdexcept>
#define VMA_IMPLEMENTATION
#define VMA_STATIC_VULKAN_FUNCTIONS 0


#include <Concerto/Core/Assert.hpp>
#include <vk_mem_alloc.h>

#include "Concerto/Graphics/Backend/Vulkan/Wrapper/Allocator.hpp"
#include "Concerto/Graphics/Backend/Vulkan/Wrapper/Instance.hpp"
#include "Concerto/Graphics/Backend/Vulkan/Wrapper/PhysicalDevice.hpp"
#include "Concerto/Graphics/Backend/Vulkan/Wrapper/Device.hpp"

namespace cct::gfx::vk
{
	Allocator::Allocator(PhysicalDevice& physicalDevice, Device& device, Instance& instance) :
		Object<VmaAllocator>(device)
	{
		VmaVulkanFunctions vulkanFunctions {
			.vkGetInstanceProcAddr = Instance::vkGetInstanceProcAddr,
			.vkGetDeviceProcAddr = instance.vkGetDeviceProcAddr,
			.vkGetPhysicalDeviceProperties = instance.vkGetPhysicalDeviceProperties,
			.vkGetPhysicalDeviceMemoryProperties = instance.vkGetPhysicalDeviceMemoryProperties,
			.vkAllocateMemory = device.vkAllocateMemory,
			.vkFreeMemory = device.vkFreeMemory,
			.vkMapMemory = device.vkMapMemory,
			.vkUnmapMemory = device.vkUnmapMemory,
			.vkFlushMappedMemoryRanges = device.vkFlushMappedMemoryRanges,
			.vkInvalidateMappedMemoryRanges = device.vkInvalidateMappedMemoryRanges,
			.vkBindBufferMemory = device.vkBindBufferMemory,
			.vkBindImageMemory = device.vkBindImageMemory,
			.vkGetBufferMemoryRequirements = device.vkGetBufferMemoryRequirements,
			.vkGetImageMemoryRequirements = device.vkGetImageMemoryRequirements,
			.vkCreateBuffer = device.vkCreateBuffer,
			.vkDestroyBuffer = device.vkDestroyBuffer,
			.vkCreateImage = device.vkCreateImage,
			.vkDestroyImage = device.vkDestroyImage,
			.vkCmdCopyBuffer = device.vkCmdCopyBuffer
		};
		VmaAllocatorCreateInfo allocatorInfo = {};
		allocatorInfo.physicalDevice = *physicalDevice.Get();
		allocatorInfo.device = *device.Get();
		allocatorInfo.instance = *instance.Get();
		allocatorInfo.pVulkanFunctions = &vulkanFunctions;

		_lastResult = vmaCreateAllocator(&allocatorInfo, &_handle);
		CCT_ASSERT(_lastResult == VK_SUCCESS, "ConcertoGraphics: Unable to create vma allocator VkResult={}", static_cast<int>(_lastResult));
	}

	Allocator::~Allocator()
	{
		if (IsNull())
			return;
		vmaDestroyAllocator(_handle);
	}

	Device& Allocator::GetDevice() const
	{
		CCT_ASSERT(_device != nullptr, "ConcertoGraphics: device is null");
		return *_device;
	}

	void Allocator::SetDevice(Device* device)
	{
		_device = device;
	}
}