//
// Created by arthur on 10/06/22.
//

#ifndef CONCERTO_GRAPHICS_VIEWPORT_HPP
#define CONCERTO_GRAPHICS_VIEWPORT_HPP

namespace Concerto::Graphics::Vk
{
	struct Viewport
	{
		float x;
		float y;
		float width;
		float height;
		float minDepth;
		float maxDepth;
	};

} // Concerto::Graphics::Vk

#endif //CONCERTO_GRAPHICS_VIEWPORT_HPP