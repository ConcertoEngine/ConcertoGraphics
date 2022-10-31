//
// Created by arthur on 25/10/2022.
//

#ifndef CONCERTOGRAPHICS_VERSION_HPP
#define CONCERTOGRAPHICS_VERSION_HPP

#include <cstdint>

namespace Concerto::Graphics
{
	struct Version
	{
		std::uint32_t major;
		std::uint32_t minor;
		std::uint32_t patch;
	};
}
#endif //CONCERTOGRAPHICS_VERSION_HPP
