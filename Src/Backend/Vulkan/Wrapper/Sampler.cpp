//
// Created by arthur on 24/10/2022.
//

#include <cassert>

#include "Concerto/Graphics/Backend/Vulkan/Wrapper/Sampler.hpp"

#include <Concerto/Core/Assert.hpp>

#include "Concerto/Graphics/Backend/Vulkan/Wrapper/Device.hpp"
#include "Concerto/Graphics/Backend/Vulkan/Wrapper/VulkanInitializer.hpp"

namespace Concerto::Graphics::Vk
{
	Sampler::Sampler(Device& device, VkFilter filter, VkSamplerAddressMode samplerAddressMode) : Object(device)
	{
		VkSamplerCreateInfo samplerInfo = VulkanInitializer::SamplerCreateInfo(filter, samplerAddressMode);
		_lastResult = _device->vkCreateSampler(*_device->Get(), &samplerInfo, nullptr, &_handle);
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
		_device->vkDestroySampler(*_device->Get(), _handle, nullptr);
	}
}
