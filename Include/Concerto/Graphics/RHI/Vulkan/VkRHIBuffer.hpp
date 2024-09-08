//
// Created by arthur on 03/09/2024.
//

#ifndef CONCERTO_GRAPHICS_RHI_VULKAN_VKRHIBUFFER_HPP
#define CONCERTO_GRAPHICS_RHI_VULKAN_VKRHIBUFFER_HPP

#include "Concerto/Graphics/RHI/Buffer.hpp"
#include "Concerto/Graphics/RHI/Vulkan/VkRHIDevice.hpp"
#include "Concerto/Graphics/Backend/Vulkan/Wrapper/Buffer.hpp"

namespace Concerto::Graphics::RHI
{
	class CONCERTO_GRAPHICS_RHI_BASE_API VkRHIBuffer final: public RHI::Buffer, public Vk::Buffer
	{
	public:
		VkRHIBuffer(VkRHIDevice& device, RHI::BufferUsageFlags usage, UInt32 allocationSize, bool allowBufferMapping);

		bool Map(Byte** data) override;
		void UnMap() override;
	private:
		bool _allowBufferMapping;
	};
}

#endif //CONCERTO_GRAPHICS_RHI_VULKAN_VKRHIBUFFER_HPP