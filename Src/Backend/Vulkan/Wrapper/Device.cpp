//
// Created by arthur on 25/10/2022.
//

#include <stdexcept>

#include <Concerto/Core/Assert.hpp>

#include "Concerto/Graphics/Backend/Vulkan/Defines.hpp"
#include <volk.h> // must be under this ^ include

#include "Concerto/Graphics/Backend/Vulkan/Wrapper/Object.hpp"
#include "Concerto/Graphics/Backend/Vulkan/Wrapper/Device.hpp"

#include "Concerto/Graphics/Backend/Vulkan/VkException.hpp"
#include "Concerto/Graphics/Backend/Vulkan/Wrapper/PhysicalDevice.hpp"
#include "Concerto/Graphics/Backend/Vulkan/Wrapper/Instance.hpp"
#include "Concerto/Graphics/Backend/Vulkan/Wrapper/ObjectDebug.hpp"

namespace cct::gfx::vk
{
	std::vector<const char*> deviceExtensions = { VK_KHR_SWAPCHAIN_EXTENSION_NAME, VK_EXT_DEBUG_MARKER_EXTENSION_NAME };

	Device::Device(PhysicalDevice& physicalDevice) :
		m_physicalDevice(&physicalDevice),
		m_allocator(nullptr)
	{
		auto lastError = Create(physicalDevice);
		if (lastError != VK_SUCCESS)
			throw VkException(lastError);
	}

	Device::~Device()
	{
		vkDestroyDevice(m_handle, nullptr);
	}

	Device::Device(Device&& other) noexcept
	{
		m_physicalDevice = std::exchange(other.m_physicalDevice, {});
		m_allocator = std::exchange(other.m_allocator, {});
		m_queues = std::exchange(other.m_queues, {});
		m_extensions = std::exchange(other.m_extensions, {});

#define CONCERTO_VULKAN_BACKEND_DEVICE_FUNCTION(func) this->func = std::exchange(other.func, nullptr);
#define CONCERTO_VULKAN_BACKEND_DEVICE_EXT_BEGIN(ext)
#define CONCERTO_VULKAN_BACKEND_DEVICE_EXT_FUNCTION(func, ...) this->func = std::exchange(other.func, nullptr);
#define CONCERTO_VULKAN_BACKEND_DEVICE_EXT_END 

#include "Concerto/Graphics/Backend/Vulkan/Wrapper/DeviceFunction.hpp"
	}

	Device& Device::operator=(Device&& other) noexcept
	{
		std::swap(m_physicalDevice, other.m_physicalDevice);
		std::swap(m_allocator, other.m_allocator);
		std::swap(m_queues, other.m_queues);
		std::swap(m_extensions, other.m_extensions);

#define CONCERTO_VULKAN_BACKEND_DEVICE_FUNCTION(func) std::swap(func, other.func);
#define CONCERTO_VULKAN_BACKEND_DEVICE_EXT_BEGIN(ext)
#define CONCERTO_VULKAN_BACKEND_DEVICE_EXT_FUNCTION(func, ...) std::swap(func, other.func);
#define CONCERTO_VULKAN_BACKEND_DEVICE_EXT_END 

#include "Concerto/Graphics/Backend/Vulkan/Wrapper/DeviceFunction.hpp"

		return *this;
	}

	VkResult Device::Create(PhysicalDevice& physicalDevice)
	{
		std::span<VkQueueFamilyProperties> queueFamilyProperties = m_physicalDevice->GetQueueFamilyProperties();
		std::vector<VkDeviceQueueCreateInfo> queueCreateInfos;
		queueCreateInfos.reserve(queueFamilyProperties.size());

		for (VkQueueFamilyProperties property : queueFamilyProperties)
		{
			auto it = std::ranges::find_if(queueCreateInfos,
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

		const VkResult result = physicalDevice.GetInstance().vkCreateDevice(*m_physicalDevice->Get(), &createInfo, nullptr, &m_handle);
		CCT_ASSERT(result == VK_SUCCESS, "Error cannot create logical device: VkResult={}", static_cast<int>(result));
		if (result != VK_SUCCESS)
			return result;

		for (auto& ext : deviceExtensions)
			m_extensions.emplace(ext);

		VolkDeviceTable deviceTable;
		volkLoadDeviceTable(&deviceTable, m_handle);
#define CONCERTO_VULKAN_BACKEND_DEVICE_FUNCTION(func) this->func = deviceTable.func;

#define CONCERTO_VULKAN_BACKEND_DEVICE_EXT_BEGIN(ext)											\
					if(IsExtensionEnabled(#ext))												\
					{
#define CONCERTO_VULKAN_BACKEND_DEVICE_EXT_FUNCTION(func, ...)									\
						CONCERTO_VULKAN_BACKEND_DEVICE_FUNCTION(func)							\
						if (this->func == nullptr)												\
						{																		\
							CCT_ASSERT_FALSE("ConcertoGraphics: Function: "						\
							#func																\
							" is null but the extension has been reported has supported");		\
						}
#define CONCERTO_VULKAN_BACKEND_DEVICE_EXT_END }

#include "Concerto/Graphics/Backend/Vulkan/Wrapper/DeviceFunction.hpp"

		CreateAllocator();

		return VK_SUCCESS;
	}

	UInt32 Device::GetQueueFamilyIndex(Queue::Type queueType) const
	{
		const std::span<VkQueueFamilyProperties> queueFamilyProperties = m_physicalDevice->GetQueueFamilyProperties();
		UInt32 i = 0;
		for (const VkQueueFamilyProperties& properties : queueFamilyProperties)
		{
			if (properties.queueFlags & VK_QUEUE_GRAPHICS_BIT && queueType == Queue::Type::Graphics)
				return i;
			else if (properties.queueFlags & VK_QUEUE_COMPUTE_BIT && queueType == Queue::Type::Compute)
				return i;
			else if (properties.queueFlags & VK_QUEUE_TRANSFER_BIT && queueType == Queue::Type::Transfer)
				return i;
			i++;
		}
		CCT_ASSERT_FALSE("No queue family found");
		return std::numeric_limits<UInt32>::max();
	}

	UInt32 Device::GetQueueFamilyIndex(UInt32 flag) const
	{
		const std::span<VkQueueFamilyProperties> queueFamilyProperties = m_physicalDevice->GetQueueFamilyProperties();
		UInt32 i = 0;
		for (const VkQueueFamilyProperties properties : queueFamilyProperties)
		{
			if (properties.queueFlags == flag && flag & VK_QUEUE_GRAPHICS_BIT)
				return i;
			if (properties.queueFlags == flag && flag & VK_QUEUE_COMPUTE_BIT)
				return i;
			if (properties.queueFlags == flag && flag & VK_QUEUE_TRANSFER_BIT)
				return i;
			++i;
		}
		CCT_ASSERT_FALSE("No queue family found");;
		return std::numeric_limits<UInt32>::max();
	}

	Queue& Device::GetQueue(Queue::Type queueType)
	{
		auto it = m_queues.find(queueType);
		if (it != m_queues.end())
			return it->second;
		auto emplace = m_queues.emplace(queueType, Queue(*this, GetQueueFamilyIndex(queueType)));
		return emplace.first->second;
	}

	void Device::WaitIdle() const
	{
		const VkResult result = this->vkDeviceWaitIdle(m_handle);
		CCT_ASSERT(result == VK_SUCCESS, "ConcertoGraphics: Failed to Wait for device idle VkResult={}", static_cast<int>(result));
	}

	void Device::UpdateDescriptorSetsWrite(std::span<VkWriteDescriptorSet> descriptorWrites) const
	{
		this->vkUpdateDescriptorSets(m_handle, static_cast<UInt32>(descriptorWrites.size()), descriptorWrites.data(), 0, nullptr);
	}

	void Device::UpdateDescriptorSetWrite(const VkWriteDescriptorSet& descriptorWrite) const
	{
		this->vkUpdateDescriptorSets(m_handle, 1, &descriptorWrite, 0, nullptr);
	}

	PhysicalDevice& Device::GetPhysicalDevice() const
	{
		CCT_ASSERT(m_physicalDevice, "ConcertoGraphics: Invalid physical device handle");
		return *m_physicalDevice;
	}

	Allocator& Device::GetAllocator() const
	{
		CCT_ASSERT(m_allocator != nullptr, "ConcertoGraphics: Allocator handle is null");
		return *m_allocator;
	}

	Instance& Device::GetInstance() const
	{
		CCT_ASSERT(m_physicalDevice, "ConcertoGraphics: Invalid physical device.");
		return m_physicalDevice->GetInstance();
	}

	bool Device::IsExtensionEnabled(const std::string& ext) const
	{
		return m_extensions.contains(ext);
	}

	void Device::CreateAllocator()
	{
		m_allocator = std::make_unique<Allocator>(*this);
		CCT_ASSERT(m_allocator != nullptr, "ConcertoGraphics: Cannot create allocator");
	}

} // cct::gfx::vk