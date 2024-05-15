//
// Created by arthur on 12/05/2024.
//

#ifndef CONCERTO_GRAPHICS_RHI_ENUMS_HPP
#define CONCERTO_GRAPHICS_RHI_ENUMS_HPP

namespace Concerto::Graphics::RHI
{
	enum class DeviceType
	{
		Other,      //Unknown device type
		Integrated, //GPU integrated to the CPU
		Dedicated,  //GPU dedicated to the graphics
		Virtual,    //Virtual GPU provided by a virtualization system
		Software    //CPU software renderer
	};

	enum class ValidationLevel
	{
		None = 0,
		Info = 1,
		Warning = 2,
		Error = 3,
		All = 4
	};
}

#endif //CONCERTO_GRAPHICS_RHI_ENUMS_HPP