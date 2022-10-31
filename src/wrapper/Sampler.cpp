//
// Created by arthur on 24/10/2022.
//

#include <cassert>

#include "wrapper/Sampler.hpp"
#include "wrapper/VulkanInitializer.hpp"

namespace Concerto::Graphics::Wrapper
{
	Sampler::Sampler(VkDevice device, VkFilter filter, VkSamplerAddressMode samplerAddressMode) : _device(device)
	{
		VkSamplerCreateInfo samplerInfo = VulkanInitializer::SamplerCreateInfo(filter, samplerAddressMode);
		if (vkCreateSampler(_device, &samplerInfo, nullptr, &_sampler) != VK_SUCCESS)
		{
			throw std::runtime_error("failed to create texture sampler!");
		}
	}

	VkSampler* Sampler::Get()
	{
		assert(_sampler != VK_NULL_HANDLE);
		return &_sampler;
	}
}
