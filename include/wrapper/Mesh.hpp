//
// Created by arthur on 09/06/22.
//

#ifndef CONCERTOGRAPHICS_MESH_HPP
#define CONCERTOGRAPHICS_MESH_HPP

#include <vector>
#include <string>
#include "AllocatedBuffer.hpp"
#include "Vertex.hpp"

namespace Concerto::Graphics::Wrapper
{

	struct Mesh {
		std::vector<Vertex> _vertices;
		AllocatedBuffer _vertexBuffer;
		bool LoadFromObj(const std::string &fileName, const std::string &materialPath);
	};

} // Concerto::Graphics::Wrapper

#endif //CONCERTOGRAPHICS_MESH_HPP
