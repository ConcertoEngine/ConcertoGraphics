//
// Created by arthur on 15/06/22.
//

#ifndef CONCERTOGRAPHICS_DESCRIPTOR_HPP
#define CONCERTOGRAPHICS_DESCRIPTOR_HPP

#include <vector>
#include "vulkan/vulkan.h"
#include "wrapper/Object.hpp"
#include "wrapper/Device.hpp"

namespace Concerto::Graphics::Wrapper
{
	class Device;

	class Descriptor : public Object<VkDescriptorPool>
	{
	public:
		Descriptor(Device& device, std::vector<VkDescriptorPoolSize> poolSizes,
				const std::vector<VkDescriptorSetLayoutBinding>& bindings);

		Descriptor(Descriptor&&) = default;

		Descriptor(const Descriptor&) = delete;

		Descriptor& operator=(Descriptor&&) = default;

		Descriptor& operator=(const Descriptor&) = delete;

		~Descriptor() = default;
	};
} // namespace Concerto::Graphics::Wrapper

#endif //CONCERTOGRAPHICS_DESCRIPTOR_HPP
