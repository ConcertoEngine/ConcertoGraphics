//
// Created by arthur on 01/09/2025.
//

#include "Concerto/Graphics/RHI/Dx12/Dx12RHIBuffer/Dx12RHIBuffer.hpp"

namespace cct::gfx::rhi
{
	bool Dx12RHIBuffer::CopyTo(const Texture& texture)
	{
		return true;
	}

	bool Dx12RHIBuffer::Map(Byte** data)
	{
		return true;
	}

	void Dx12RHIBuffer::UnMap()
	{
	}
}
