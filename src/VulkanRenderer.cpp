//
// Created by arthur on 25/10/2022.
//

#include <cassert>
#include "VulkanRenderer.hpp"
namespace Concerto
{
	Graphics::VulkanRenderer* Graphics::VulkanRenderer::Instance()
	{
		assert(_instance != nullptr);
		return _instance;
	}
} // Concerto