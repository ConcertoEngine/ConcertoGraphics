//
// Created by arthur on 03/09/2024.
//

#ifndef CONCERTO_GRAPHICS_RHI_VKRHICOMMANDBUFFER_HPP
#define CONCERTO_GRAPHICS_RHI_VKRHICOMMANDBUFFER_HPP

#include "Concerto/Graphics/RHI/Vulkan/VkRHIDevice.hpp"
#include "Concerto/Graphics/RHI/CommandBuffer.hpp"
#include "Concerto/Graphics/Backend/Vulkan/Wrapper/CommandBuffer.hpp"

namespace Concerto::Graphics::RHI
{
	class CONCERTO_GRAPHICS_VKRHI_API VkRHICommandBuffer final: public RHI::CommandBuffer, public Vk::CommandBuffer
	{
	public:
		VkRHICommandBuffer(VkRHIDevice& device, Vk::CommandPool& commandPool);

		void Begin() override;
		void End() override;
		void Submit() override;
		void Reset() override;
	};
}

#endif //CONCERTO_GRAPHICS_RHI_VKRHICOMMANDBUFFER_HPP