//
// Created by arthur on 24/10/2022.
//

#ifndef CONCERTOGRAPHICS_SAMPLER_HPP
#define CONCERTOGRAPHICS_SAMPLER_HPP

#include <vulkan/vulkan.h>
#include <Concerto/Core/Types.hpp>

#include "Concerto/Graphics/Vulkan/Wrapper/Object.hpp"

namespace Concerto::Graphics
{
	class Device;

	/**
	 * @class Sampler
	 * @brief A class representing a sampler.
	 *
	 * A sampler is an object that describes how sampled images should be read in a shader. This class will create the VkSampler object, and it's default move and copy constructors and assignment operators are provided.
	 */
	class CONCERTO_PUBLIC_API Sampler : public Object<VkSampler>
	{
	public:
		/**
		 * @brief Constructs a Sampler object.
		 * @param device The Device object associated with the sampler.
		 * @param filter The filter to use when sampling the image.
		 * @param samplerAddressMode The addressing mode to use for the sampler.
		 */
		Sampler(Device& device, VkFilter filter,
				VkSamplerAddressMode samplerAddressMode = VK_SAMPLER_ADDRESS_MODE_REPEAT);
	};
}


#endif //CONCERTOGRAPHICS_SAMPLER_HPP
