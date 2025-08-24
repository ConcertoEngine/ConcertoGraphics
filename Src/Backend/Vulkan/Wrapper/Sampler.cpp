//
// Created by arthur on 24/10/2022.
//

#include <cassert>

#include "Concerto/Graphics/Backend/Vulkan/Wrapper/Sampler.hpp"

#include <Concerto/Core/Assert.hpp>

#include "Concerto/Graphics/Backend/Vulkan/Wrapper/Device.hpp"
#include "Concerto/Graphics/Backend/Vulkan/Wrapper/VulkanInitializer.hpp"

namespace cct::gfx::vk
{
	Sampler::Sampler(Device& device, VkFilter filter, VkSamplerAddressMode samplerAddressMode) : Object(device)
	{
		VkSamplerCreateInfo samplerInfo = VulkanInitializer::SamplerCreateInfo(filter, samplerAddressMode);
		m_lastResult = m_device->vkCreateSampler(*m_device->Get(), &samplerInfo, nullptr, &m_handle);
		if (m_lastResult != VK_SUCCESS)
		{
			CCT_ASSERT_FALSE("ConcertoGraphics: vkCreateSampler failed VKResult={}", static_cast<int>(m_lastResult));
			return;
		}
	}

	Sampler::~Sampler()
	{
		if (IsNull())
			return;
		m_device->vkDestroySampler(*m_device->Get(), m_handle, nullptr);
	}
}
