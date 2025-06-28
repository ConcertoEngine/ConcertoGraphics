//
// Created by arthur on 17/02/2023.
//

#include <Concerto/Core/Cast.hpp>

#include "Concerto/Graphics/RHI/Vulkan/VkRHITextureBuilder.hpp"

#include "Concerto/Graphics/RHI/Vulkan/VKRHITexture.hpp"
#include "Concerto/Graphics/RHI/Vulkan/VkRHIDevice.hpp"
#include "Concerto/Graphics/Backend/Vulkan/Wrapper/Device.hpp"
#include "Concerto/Graphics/Backend/Vulkan/Wrapper/CommandBuffer.hpp"
#include "Concerto/Graphics/Backend/Vulkan/UploadContext.hpp"

namespace cct::gfx::rhi
{
	VkRHITextureBuilder* VkRHITextureBuilder::_instance = nullptr;

	VkRHITextureBuilder::VkRHITextureBuilder(rhi::Device& device, vk::UploadContext& uploadContext, vk::Queue& queue) :
		TextureBuilder(device),
		_device(device),
		_uploadContext(uploadContext),
		_queue(queue)
	{
		if (_instance != nullptr)
		{
			CCT_ASSERT_FALSE("ConcertoGraphics: Texture builder instance is null");
			return;
		}
		_instance = this;
	}

	VkRHITextureBuilder::~VkRHITextureBuilder()
	{
		_instance = nullptr;
	}

	VkRHITextureBuilder* VkRHITextureBuilder::Instance()
	{
		CCT_ASSERT(_instance, "ConcertoGraphics: VkRHITextureBuilder instance is null");
		return _instance;
	}

	std::shared_ptr<Texture> VkRHITextureBuilder::BuildApiTexture(PixelFormat format, Int32 width, Int32 height)
	{
		return std::make_shared<VkRHITexture>(Cast<VkRHIDevice&>(_device), format, width, height, VK_IMAGE_ASPECT_COLOR_BIT);
	}

	void VkRHITextureBuilder::InternalCommit()
	{
		auto& vkDevice = cct::Cast<VkRHIDevice&>(_device);
		_uploadContext._commandBuffer.Begin();
		_uploadContext.ExecuteSecondaryCommandBuffers();
		_uploadContext._commandBuffer.End();

		vkDevice.GetQueue(vk::Queue::Type::Transfer).Submit(_uploadContext._commandBuffer, nullptr, nullptr, _uploadContext._uploadFence);
		_uploadContext._uploadFence.Wait(9999999999);
		_uploadContext._uploadFence.Reset();
		_uploadContext._commandPool.Reset();
	}
}
