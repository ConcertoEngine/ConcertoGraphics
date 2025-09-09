//
// Created by arthur on 01/09/2025.
//

#include "Concerto/Graphics/RHI/Dx12/Dx12RHICommandPool/Dx12RHICommandPool.hpp"
#include "Concerto/Graphics/RHI/Dx12/Dx12RHICommandBuffer/Dx12RHICommandBuffer.hpp"

namespace cct::gfx::rhi
{

	Dx12RHICommandPool::Dx12RHICommandPool(CommandBufferUsage usage) :
		CommandPool(usage)
	{
	}

	std::unique_ptr<CommandBuffer> Dx12RHICommandPool::AllocateCommandBuffer()
	{
		return std::make_unique<Dx12RHICommandBuffer>();
	}
}
