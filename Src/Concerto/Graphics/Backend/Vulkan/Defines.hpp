//
// Created by arthur on 24/08/2023.
//

#ifndef CONCERTO_GRAPHICS_BACKEND_VULKAN_DEFINES_HPP
#define CONCERTO_GRAPHICS_BACKEND_VULKAN_DEFINES_HPP

#include <Concerto/Core/Types.hpp>

#ifdef CCT_COMPILER_MSVC
#pragma warning(disable: 4251) // Disable warning about DLL interface needed
#endif

#ifdef CONCERTO_GRAPHICS_VULKAN_BACKEND_BUILD
	#define CONCERTO_GRAPHICS_VULKAN_BACKEND_API CCT_EXPORT
#else
#define CONCERTO_GRAPHICS_VULKAN_BACKEND_API CCT_IMPORT
#endif // CONCERTO_GRAPHICS_VULKAN_BACKEND_BUILD
#define VOLK_VULKAN_H_PATH <string>
#include <vulkan/vk_platform.h>
#include <vulkan/vulkan_core.h>

#ifdef CCT_PLATFORM_WINDOWS
	typedef unsigned long DWORD;
	typedef const wchar_t* LPCWSTR;
	typedef void* HANDLE;
	typedef struct HINSTANCE__* HINSTANCE;
	typedef struct HWND__* HWND;
	typedef struct HMONITOR__* HMONITOR;
	typedef struct m_SECURITY_ATTRIBUTES SECURITY_ATTRIBUTES;
	#define VK_USE_PLATFORM_WIN32_KHR
	#include <vulkan/vulkan_win32.h>
#else
#include <vulkan/vuklkan.h>
#endif

#define VULKAN_H_ // define VULKAN_H_ to avoid third party libraries to include <vulkan/vulkan.h>

#include <Concerto/Graphics/Profiler/Profiler.hpp>
#include <Concerto/Core/Result.hpp>

#endif //CONCERTO_GRAPHICS_BACKEND_VULKAN_DEFINES_HPP