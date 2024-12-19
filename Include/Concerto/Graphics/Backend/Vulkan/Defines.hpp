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

#include <vulkan/vk_platform.h>
#include <vulkan/vulkan_core.h>

#ifdef CCT_PLATFORM_WINDOWS
	#define VK_USE_PLATFORM_WIN32_KHR
	#include <windows.h>
	#undef max
	#undef min
	#undef GetObject
	#include <vulkan/vulkan_win32.h>
#endif


#endif //CONCERTO_GRAPHICS_BACKEND_VULKAN_DEFINES_HPP