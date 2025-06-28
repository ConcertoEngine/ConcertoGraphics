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
		_backend(backend)
	{
		CCT_GFX_AUTO_PROFILER_SCOPE();
		switch (backend)
		{
		case Backend::ConcertoVulkan:
			_apiImpl = std::make_unique<VkRHI>();
			_apiImpl->Create(validationLevel);
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
		CCT_ASSERT(_apiImpl, "ConcertoGraphics: Tying to get an invalid ApiImpl");
		return _apiImpl->EnumerateDevices();
	}

	std::unique_ptr<Device> Instance::CreateDevice(std::size_t index)
	{
		CCT_ASSERT(_apiImpl, "ConcertoGraphics: Tying to get an invalid ApiImpl");
		return _apiImpl->CreateDevice(index);
	}

	APIImpl* Instance::GetImpl()
	{
		CCT_ASSERT(_apiImpl, "ConcertoGraphics: Tying to get an invalid ApiImpl");
		return _apiImpl.get();
	}
}
