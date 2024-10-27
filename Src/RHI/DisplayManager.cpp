//
// Created by arthur on 27/10/2024.
//

#include <SDL2/SDL.h>
#include <Concerto/Core/Assert.hpp>

#include "Concerto/Graphics/DisplayManager.hpp"


namespace Concerto::Graphics
{
	DisplayManager::DisplayManager()
	{
		int result = SDL_Init(SDL_INIT_VIDEO);
		if (result < 0)
		{
			CONCERTO_ASSERT_FALSE("ConcertoGraphics: SDL initialization failed error code: {}, message {}", result, SDL_GetError());
			throw std::runtime_error(std::format("SDL initialization failed error code: {}, message {}", result, SDL_GetError()));
		}
	}

	DisplayManager::~DisplayManager()
	{
		SDL_Quit();
	}
}
