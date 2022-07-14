//
// Created by arthur on 14/07/2022.
//
#include "glm/glm.hpp"
#ifndef CONCERTOGRAPHICS_MESHPUSHCONSTANTS_HPP
#define CONCERTOGRAPHICS_MESHPUSHCONSTANTS_HPP

struct MeshPushConstants
{
	glm::vec4 data;
	glm::mat4 render_matrix;
};

#endif //CONCERTOGRAPHICS_MESHPUSHCONSTANTS_HPP
