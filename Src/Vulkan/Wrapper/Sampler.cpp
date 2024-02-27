//
// Created by arthur on 24/10/2022.
//

#include <cassert>

#include "Concerto/Graphics/Vulkan/Wrapper/Sampler.hpp"

#include <Concerto/Core/Assert.hpp>

#include "Concerto/Graphics/Vulkan/Wrapper/Device.hpp"
#include "Concerto/Graphics/Vulkan/Wrapper/VulkanInitializer.hpp"

namespace Concerto::Graphics
{
	Sampler::Sampler(Device& device, VkFilter filter, VkSamplerAddressMode samplerAddressMode) : Object(device)
	{
		VkSamplerCreateInfo samplerInfo = VulkanInitializer::SamplerCreateInfo(filter, samplerAddressMode);
		if (vkCreateSampler(*_device->Get(), &samplerInfo, nullptr, &_handle) != VK_SUCCESS)
		{
			CONCERTO_ASSERT_FALSE;
			throw std::runtime_error("failed to create texture sampler!");
		}
	}

	Sampler::~Sampler()
	{
		if (IsNull())
			return;
		vkDestroySampler(*_device->Get(), _handle, nullptr);
	}
}
