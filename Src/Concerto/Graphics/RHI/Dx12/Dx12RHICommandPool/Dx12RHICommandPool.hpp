//
// Created by arthur on 01/09/2025.
//

#ifndef CONCERTO_GRAPHICS_RHI_DX12_DX12RHICOMMANDPOOL_HPP
#define CONCERTO_GRAPHICS_RHI_DX12_DX12RHICOMMANDPOOL_HPP

#include "Concerto/Graphics/RHI/CommandPool.hpp"

namespace cct::gfx::rhi
{
	class CONCERTO_GRAPHICS_RHI_BASE_API Dx12RHICommandPool : public rhi::CommandPool
	{
	public:
		Dx12RHICommandPool();

		std::unique_ptr<CommandBuffer> AllocateCommandBuffer(CommandBufferUasge usage) override;
	};
}

#endif //CONCERTO_GRAPHICS_RHI_DX12_DX12RHICOMMANDPOOL_HPP