//
// Created by arthur on 28/10/2024.
//

#ifndef CONCERTO_GRAPHICS_NATIVEWINDOW_HPP
#define CONCERTO_GRAPHICS_NATIVEWINDOW_HPP

#include "Concerto/Graphics/Core/Defines.hpp"

namespace cct::gfx
{

#if defined(CCT_PLATFORM_WINDOWS)
	struct NativeWindow
	{
		void* window;
		void* hinstance;
	};
#elif defined(CCT_PLATFORM_MACOS)
	struct NativeWindow
	{
		//Not implemented
	};
#elif defined(CCT_PLATFORM_LINUX)
	struct NativeWindow
	{
		//Not implemented
	};
#endif
}

#endif //CONCERTO_GRAPHICS_NATIVEWINDOW_HPP