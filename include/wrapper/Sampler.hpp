//
// Created by arthur on 24/10/2022.
//

#ifndef CONCERTOGRAPHICS_SAMPLER_HPP
#define CONCERTOGRAPHICS_SAMPLER_HPP

#include "vulkan/vulkan.h"
#include "wrapper/Object.hpp"

namespace Concerto::Graphics::Wrapper
{
	class Device;

	class Sampler : public Object<VkSampler>
	{
	public:
		Sampler(Device& device, VkFilter filter,
				VkSamplerAddressMode samplerAddressMode = VK_SAMPLER_ADDRESS_MODE_REPEAT);
	};
}


#endif //CONCERTOGRAPHICS_SAMPLER_HPP
