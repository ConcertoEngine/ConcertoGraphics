//
// Created by arthur on 17/02/2023.
//

#include <Concerto/Core/Cast.hpp>

#include "Concerto/Graphics/RHI/Vulkan/VkRHITextureBuilder.hpp"

#include "Concerto/Graphics/Backend/Vulkan/Wrapper/Device.hpp"
#include "Concerto/Graphics/Backend/Vulkan/Wrapper/CommandBuffer.hpp"
#include "Concerto/Graphics/RHI/Vulkan/VKRHITexture.hpp"
#include "Concerto/Graphics/Backend/Vulkan/UploadContext.hpp"
#include "Concerto/Graphics/RHI/Vulkan/VkRHIDevice.hpp"

namespace Concerto::Graphics::RHI
{
	VkRHITextureBuilder* VkRHITextureBuilder::_instance = nullptr;

	VkRHITextureBuilder::VkRHITextureBuilder(RHI::Device& device, Vk::UploadContext& uploadContext, Vk::Queue& queue) :
		_device(device),
		_uploadContext(uploadContext),
		_queue(queue)
	{
		if (_instance != nullptr)
		{
			CONCERTO_ASSERT_FALSE("ConcertoGraphics: Texture builder instance is null");
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
		CONCERTO_ASSERT(_instance, "ConcertoGraphics: VkRHITextureBuilder instance is null");
		return _instance;
	}

	std::shared_ptr<Texture> VkRHITextureBuilder::BuildTexture(const std::string& path)
	{
		auto it = _texturesCache.find(path);
		if (it != _texturesCache.end())
			return it->second;

		auto texture = std::make_shared<VkRHITexture>(Cast<VkRHIDevice&>(_device),
			path,
			_uploadContext._commandBuffer,
			_uploadContext,
			_queue,
			VK_IMAGE_ASPECT_COLOR_BIT);
		_texturesCache[path] = texture;
		return texture;
	}
}
