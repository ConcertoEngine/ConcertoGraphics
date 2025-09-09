//
// Created by arthur on 01/09/2025.
//

#ifndef CONCERTO_GRAPHICS_RHI_DX12_DX12RHIBUFFER_HPP
#define CONCERTO_GRAPHICS_RHI_DX12_DX12RHIBUFFER_HPP

#include "Concerto/Graphics/RHI/Buffer.hpp"

namespace cct::gfx::rhi
{
	class CONCERTO_GRAPHICS_RHI_BASE_API Dx12RHIBuffer : public rhi::Buffer
	{
	public:
		Dx12RHIBuffer();

		bool CopyTo(const Texture& texture) override;
		bool Map(Byte** data) override;
		void UnMap() override;
	};
}

#endif //CONCERTO_GRAPHICS_RHI_DX12_DX12RHIBUFFER_HPP