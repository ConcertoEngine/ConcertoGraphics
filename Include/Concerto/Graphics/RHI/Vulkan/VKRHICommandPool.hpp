//
// Created by arthur on 03/09/2024.
//

#ifndef CONCERTO_GRAPHICS_RHI_VKRHICOMMANDPOOL_HPP
#define CONCERTO_GRAPHICS_RHI_VKRHICOMMANDPOOL_HPP

#include "Concerto/Graphics/RHI/Vulkan/VkRHIDevice.hpp"
#include "Concerto/Graphics/RHI/CommandPool.hpp"
#include "Concerto/Graphics/Backend/Vulkan/Wrapper/CommandPool.hpp"

namespace Concerto::Graphics::RHI
{
	class CONCERTO_GRAPHICS_VKRHI_API VkRHICommandPool final: public CommandPool, public Vk::CommandPool
	{
	public:
		VkRHICommandPool(VkRHIDevice& device, RHI::QueueFamily family);

		std::unique_ptr<CommandBuffer> AllocateCommandBuffer(CommandBufferUasge usage) override;
	private:
		VkRHIDevice& _device;
	};
}

#endif //CONCERTO_GRAPHICS_RHI_VKRHICOMMANDPOOL_HPP