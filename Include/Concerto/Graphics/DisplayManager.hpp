//
// Created by arthur on 27/10/2024.
//

#ifndef CONCERTO_GRAPHICS_DISPLAYMANAGER_HPP
#define CONCERTO_GRAPHICS_DISPLAYMANAGER_HPP

#include <vector>

#include "Concerto/Graphics/PixelFormat.hpp"
#include "Concerto/Graphics/Window/Window.hpp"
#include "Concerto/Graphics/Defines.hpp"

namespace Concerto::Graphics
{
	struct DisplayInfo
	{
		struct Bounds
		{
			Int32 x, y;
			Int32 w, h;
		};

		struct DisplayMode
		{
			Int32 displayModeIndex;
			PixelFormat pixelFormat;
			Int32 width;
			Int32 height;
			Int32 refreshRate;
		};

		Int32 displayIndex;
		std::string_view displayName;
		Bounds displayBounds;
		bool isPrimary;

		std::vector<DisplayMode> displayModes;
	};

	class CONCERTO_GRAPHICS_API DisplayManager
	{
	public:
		DisplayManager();
		~DisplayManager();

		std::vector<DisplayInfo> EnumerateDisplaysInfos();

		std::unique_ptr<Window> CreateWindow(Int32 displayIndex, const std::string& name, Int32 width, Int32 height);
	private:
	};
}

#endif //CONCERTO_GRAPHICS_DISPLAYMANAGER_HPP