//
// Created by arthur on 15/04/2023.
//

#ifndef BASICRENDERING_INCLUDE_CONCERTO_GRAPHICS_PRIMITIVES_PRIMITIVES_HPP_
#define BASICRENDERING_INCLUDE_CONCERTO_GRAPHICS_PRIMITIVES_PRIMITIVES_HPP_
#include "Concerto/Graphics/Defines.hpp"
#include <Concerto/Core/Math/Vector.hpp>

#include "Concerto/Graphics/Vulkan/Vertex.hpp"

namespace Concerto::Graphics
{
	struct CONCERTO_GRAPHICS_API Primitive
	{
		static Vertices MakeCone(float radius, float height, UInt32 slices);
		static Vertices MakeCube(float size);
		static Vertices MakeSphere(float radius, UInt32 slices, UInt32 stacks);
		static Vertices MakeCylinder(float radius, float height, UInt32 slices);
		static Vertices MakePlane(float size);
	};
}
#endif //BASICRENDERING_INCLUDE_CONCERTO_GRAPHICS_PRIMITIVES_PRIMITIVES_HPP_
