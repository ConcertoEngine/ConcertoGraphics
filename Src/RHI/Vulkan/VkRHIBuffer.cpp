//
// Created by arthur on 03/09/2024.
//

#include "Concerto/Graphics/RHI/Vulkan/VkRHIBuffer.hpp"
#include "Concerto/Graphics/RHI/Vulkan/Utils.hpp"

namespace Concerto::Graphics::RHI
{
	VkRHIBuffer::VkRHIBuffer(VkRHIDevice& device, RHI::BufferUsageFlags usage, UInt32 allocationSize, bool allowBufferMapping) :
		Vk::Buffer(device.GetAllocator(), allocationSize, Converters::ToVulkan(usage), VMA_MEMORY_USAGE_AUTO, allowBufferMapping),
		_allowBufferMapping(allowBufferMapping)
	{
	}

	bool VkRHIBuffer::Map(Byte** data)
	{
		if (!_allowBufferMapping)
		{
			CONCERTO_ASSERT_FALSE("ConcertoGraphics: buffer mapping is not enabled");
			return false;
		}
		return Vk::Buffer::Map(data);;
	}

	void VkRHIBuffer::UnMap()
	{
		if (!_allowBufferMapping)
		{
			CONCERTO_ASSERT_FALSE("ConcertoGraphics: buffer mapping is not enabled");
			return;
		}
		Vk::Buffer::UnMap();
	}
}
