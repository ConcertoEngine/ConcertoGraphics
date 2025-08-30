//
// Created by arthur on 17/02/2023.
//

#include <Concerto/Core/Cast.hpp>

#include "Concerto/Graphics/RHI/Vulkan/VkRHITextureBuilder/VkRHITextureBuilder.hpp"
#include "Concerto/Graphics/RHI/Vulkan/VKRHITexture/VKRHITexture.hpp"
#include "Concerto/Graphics/RHI/Vulkan/VkRHIDevice/VkRHIDevice.hpp"

#include "Concerto/Graphics/Backend/Vulkan/Wrapper/Device/Device.hpp"
#include "Concerto/Graphics/Backend/Vulkan/Wrapper/CommandBuffer/CommandBuffer.hpp"
#include "Concerto/Graphics/Backend/Vulkan/UploadContext/UploadContext.hpp"

namespace cct::gfx::rhi
{
	VkRHITextureBuilder* VkRHITextureBuilder::m_instance = nullptr;

	VkRHITextureBuilder::VkRHITextureBuilder(rhi::Device& device, vk::UploadContext& uploadContext, vk::Queue& queue) :
		TextureBuilder(device),
		m_device(device),
		m_uploadContext(uploadContext),
		m_queue(queue)
	{
		if (m_instance != nullptr)
		{
			CCT_ASSERT_FALSE("ConcertoGraphics: Texture builder instance is null");
			return;
		}
		m_instance = this;
	}

	VkRHITextureBuilder::~VkRHITextureBuilder()
	{
		m_instance = nullptr;
	}

	VkRHITextureBuilder* VkRHITextureBuilder::Instance()
	{
		CCT_ASSERT(m_instance, "ConcertoGraphics: VkRHITextureBuilder instance is null");
		return m_instance;
	}

	std::shared_ptr<Texture> VkRHITextureBuilder::BuildApiTexture(PixelFormat format, Int32 width, Int32 height)
	{
		return std::make_shared<VkRHITexture>(Cast<VkRHIDevice&>(m_device), format, width, height, VK_IMAGE_ASPECT_COLOR_BIT);
	}

	void VkRHITextureBuilder::InternalCommit()
	{
		auto& vkDevice = cct::Cast<VkRHIDevice&>(m_device);
		m_uploadContext.m_commandBuffer.Begin();
		m_uploadContext.ExecuteSecondaryCommandBuffers();
		m_uploadContext.m_commandBuffer.End();

		vkDevice.GetQueue(vk::Queue::Type::Transfer).Submit(m_uploadContext.m_commandBuffer, nullptr, nullptr, m_uploadContext.m_uploadFence);
		m_uploadContext.m_uploadFence.Wait(9999999999);
		m_uploadContext.m_uploadFence.Reset();
		m_uploadContext.m_commandPool.Reset();
	}
}
