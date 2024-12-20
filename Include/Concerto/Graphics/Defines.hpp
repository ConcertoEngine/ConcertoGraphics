//
// Created by arthur on 24/08/2023.
//

#ifndef CONCERTO_GRAPHICS_DEFINES_HPP
#define CONCERTO_GRAPHICS_DEFINES_HPP

#include <Concerto/Core/Types.hpp>

#ifdef CCT_COMPILER_MSVC
#pragma warning(disable: 4251) // Disable warning about DLL interface needed
#endif

#ifdef CONCERTO_GRAPHICS_BUILD
	#define CONCERTO_GRAPHICS_API CCT_EXPORT
#else
	#define CONCERTO_GRAPHICS_API CCT_IMPORT
#endif // CONCERTO_GRAPHICS_BUILD

#endif //CONCERTO_GRAPHICS_DEFINES_HPP