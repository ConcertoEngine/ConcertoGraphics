//
// Created by arthur on 03/09/2024.
//

#ifndef CONCERTO_GRAPHICS_RHI_VKRHICOMMANDPOOL_HPP
#define CONCERTO_GRAPHICS_RHI_VKRHICOMMANDPOOL_HPP

#include "Concerto/Graphics/RHI/Vulkan/VkRHIDevice.hpp"
#include "Concerto/Graphics/RHI/CommandPool.hpp"
#include "Concerto/Graphics/Backend/Vulkan/Wrapper/CommandPool.hpp"

namespace cct::gfx::rhi
{
	class CONCERTO_GRAPHICS_RHI_BASE_API VkRHICommandPool final: public CommandPool, public vk::CommandPool
	{
	public:
		VkRHICommandPool(VkRHIDevice& device, rhi::QueueFamily family);

		std::unique_ptr<CommandBuffer> AllocateCommandBuffer(CommandBufferUasge usage) override;
	private:
		VkRHIDevice& _device;
	};
}

#endif //CONCERTO_GRAPHICS_RHI_VKRHICOMMANDPOOL_HPP