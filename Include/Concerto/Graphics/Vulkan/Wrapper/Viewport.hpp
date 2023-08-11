//
// Created by arthur on 10/06/22.
//

#ifndef CONCERTOGRAPHICS_VIEWPORT_HPP
#define CONCERTOGRAPHICS_VIEWPORT_HPP

namespace Concerto::Graphics
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

} // Concerto::Graphics::Wrapper

#endif //CONCERTOGRAPHICS_VIEWPORT_HPP