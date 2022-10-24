//
// Created by arthur on 24/10/2022.
//

#ifndef CONCERTOGRAPHICS_SAMPLER_HPP
#define CONCERTOGRAPHICS_SAMPLER_HPP

#include "vulkan/vulkan.h"

namespace Concerto::Graphics::Wrapper
{
	class Sampler
	{
	public:
		Sampler(VkDevice device, VkFilter filter, VkSamplerAddressMode samplerAddressMode = VK_SAMPLER_ADDRESS_MODE_REPEAT);
		VkSampler* Get();
	private:
		VkDevice _device;
		VkSampler _sampler;
	};
}


#endif //CONCERTOGRAPHICS_SAMPLER_HPP
