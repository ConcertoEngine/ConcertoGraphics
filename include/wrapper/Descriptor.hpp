//
// Created by arthur on 15/06/22.
//

#ifndef CONCERTOGRAPHICS_DESCRIPTOR_HPP
#define CONCERTOGRAPHICS_DESCRIPTOR_HPP

#include "vulkan/vulkan.h"
#include <vector>

namespace Concerto::Graphics::Wrapper
{
	class Descriptor
	{
	public:
		Descriptor(VkDevice device, std::vector<VkDescriptorPoolSize> poolSizes, std::vector<VkDescriptorSetLayoutBinding> bindings);

		Descriptor(Descriptor&&) = default;

		Descriptor(const Descriptor&) = delete;

		Descriptor& operator=(Descriptor&&) = default;

		Descriptor& operator=(const Descriptor&) = delete;

		~Descriptor() = default;

		VkDescriptorPool Get() const;

	private:
		VkDescriptorPool _pool;
	};
} // namespace Concerto::Graphics::Wrapper

#endif //CONCERTOGRAPHICS_DESCRIPTOR_HPP
