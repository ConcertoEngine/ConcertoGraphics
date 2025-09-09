//
// Created by arthur on 01/09/2025.
//

#ifndef CONCERTO_GRAPHICS_RHI_DX12_DX12RHICOMMANDPOOL_HPP
#define CONCERTO_GRAPHICS_RHI_DX12_DX12RHICOMMANDPOOL_HPP

#include "Concerto/Graphics/Backend/Dx12/Wrapper/CommandAllocator/CommandAllocator.hpp"
#include "Concerto/Graphics/RHI/CommandPool.hpp"

namespace cct::gfx::rhi
{
	class CONCERTO_GRAPHICS_RHI_BASE_API Dx12RHICommandPool : public rhi::CommandPool, public dx12::CommandAllocator
	{
	public:
		Dx12RHICommandPool(CommandBufferUsage usage);

		std::unique_ptr<CommandBuffer> AllocateCommandBuffer() override;
	};
}

#endif //CONCERTO_GRAPHICS_RHI_DX12_DX12RHICOMMANDPOOL_HPP