//
// Created by arthur on 201/09/2025.
//

#ifndef CONCERTO_GRAPHICS_BACKEND_DX12_DEFINES_HPP
#define CONCERTO_GRAPHICS_BACKEND_DX12_DEFINES_HPP

#include <Concerto/Core/Types.hpp>

#ifdef CCT_COMPILER_MSVC
#pragma warning(disable: 4251) // Disable warning about DLL interface needed
#endif

#ifdef CONCERTO_GRAPHICS_DX12_BACKEND_BUILD
	#define CONCERTO_GRAPHICS_DX12_BACKEND_API CCT_EXPORT
#else
    #define CONCERTO_GRAPHICS_DX12_BACKEND_API CCT_IMPORT
#endif // CONCERTO_GRAPHICS_DX12_BACKEND_BUILD

#include <Windows.h>
#include <wrl.h>
#include <dxgi.h>
#include <dxgi1_3.h>
#include <dxgi1_4.h>
#include <d3d12.h>
#undef min
#undef max
#undef OutputDebugString

#include <vector>

#include <Concerto/Core/Assert.hpp>
#include "Concerto/Graphics/Backend/Dx12/Dx12Exception.hpp"

#endif //CONCERTO_GRAPHICS_BACKEND_DX12_DEFINES_HPP