//
// Created by arthur on 12/05/2024.
//

#include <Concerto/Core/Assert.hpp>
#include "Concerto/Graphics/RHI/Instance.hpp"
#include "Concerto/Graphics/RHI/APIImpl.hpp"
#include "Concerto/Graphics/RHI/Vulkan/VkRHI.hpp"

namespace cct::gfx::rhi
{
	Instance::Instance(Backend backend, ValidationLevel validationLevel) :
		m_backend(backend)
	{
		CCT_GFX_AUTO_PROFILER_SCOPE();
		switch (backend)
		{
		case Backend::ConcertoVulkan:
			m_apiImpl = std::make_unique<VkRHI>();
			m_apiImpl->Create(validationLevel);
			break;
		case Backend::Vulkan:
		case Backend::DirectX11:
		case Backend::DirectX12:
		case Backend::Metal:
		case Backend::OpenGL:
		case Backend::OpenGLES:
			CCT_ASSERT_FALSE("Not implemented");
			break;
		}
	}

	std::span<const DeviceInfo> Instance::EnumerateDevices() const
	{
		CCT_ASSERT(m_apiImpl, "ConcertoGraphics: Tying to get an invalid ApiImpl");
		return m_apiImpl->EnumerateDevices();
	}

	std::unique_ptr<Device> Instance::CreateDevice(std::size_t index) const
	{
		CCT_ASSERT(m_apiImpl, "ConcertoGraphics: Tying to get an invalid ApiImpl");
		return m_apiImpl->CreateDevice(index);
	}

	APIImpl* Instance::GetImpl() const
	{
		CCT_ASSERT(m_apiImpl, "ConcertoGraphics: Tying to get an invalid ApiImpl");
		return m_apiImpl.get();
	}
}
