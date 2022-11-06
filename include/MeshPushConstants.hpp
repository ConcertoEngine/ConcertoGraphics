//
// Created by arthur on 14/07/2022.
//
#ifndef CONCERTOGRAPHICS_MESHPUSHCONSTANTS_HPP
#define CONCERTOGRAPHICS_MESHPUSHCONSTANTS_HPP
#include "glm/vec4.hpp"
#include "glm/mat4x4.hpp"

struct MeshPushConstants
{
	glm::vec4 data;
	glm::mat4 render_matrix;
};

#endif //CONCERTOGRAPHICS_MESHPUSHCONSTANTS_HPP
