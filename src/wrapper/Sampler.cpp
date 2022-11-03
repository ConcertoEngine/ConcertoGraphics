//
// Created by arthur on 24/10/2022.
//

#include <cassert>

#include "wrapper/Sampler.hpp"
#include "wrapper/Device.hpp"
#include "wrapper/VulkanInitializer.hpp"

namespace Concerto::Graphics::Wrapper
{
	Sampler::Sampler(Device& device, VkFilter filter, VkSamplerAddressMode samplerAddressMode) : Object<VkSampler>(device)
	{
		VkSamplerCreateInfo samplerInfo = VulkanInitializer::SamplerCreateInfo(filter, samplerAddressMode);
		if (vkCreateSampler(*_device->Get(), &samplerInfo, nullptr, &_handle) != VK_SUCCESS)
		{
			throw std::runtime_error("failed to create texture sampler!");
		}
	}
}
