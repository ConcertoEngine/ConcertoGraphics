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
		_lastResult = vkCreateSampler(*_device->Get(), &samplerInfo, nullptr, &_handle);
		if (_lastResult != VK_SUCCESS)
		{
			CONCERTO_ASSERT_FALSE("ConcertoGraphics: vkCreateSampler failed VKResult={}", static_cast<int>(_lastResult));
			return;
		}
	}

	Sampler::~Sampler()
	{
		if (IsNull())
			return;
		vkDestroySampler(*_device->Get(), _handle, nullptr);
	}
}
