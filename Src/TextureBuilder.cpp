//
// Created by arthur on 17/02/2023.
//

#include "Concerto/Graphics/TextureBuilder.hpp"

#include "Concerto/Graphics/Vulkan/Wrapper/Device.hpp"
#include "Concerto/Graphics/Vulkan/Wrapper/CommandBuffer.hpp"
#include "Concerto/Graphics/Vulkan/Wrapper/Queue.hpp"
#include "Concerto/Graphics/Vulkan/Texture.hpp"
#include "Concerto/Graphics/UploadContext.hpp"

namespace Concerto::Graphics
{
	TextureBuilder* TextureBuilder::_instance = nullptr;

	TextureBuilder::TextureBuilder(Device& device, Allocator& allocator,
		CommandBuffer& commandBuffer, UploadContext& uploadContext, Queue& queue) :
		_device(device),
		_commandBuffer(commandBuffer),
		_uploadContext(uploadContext),
		_queue(queue)
	{
		if (_instance != nullptr)
		{
			CONCERTO_ASSERT_FALSE;
			return;
		}
		_instance = this;
	}

	TextureBuilder::~TextureBuilder()
	{
		_instance = nullptr;
	}

	TextureBuilder* TextureBuilder::Instance()
	{
		CONCERTO_ASSERT(_instance);
		return _instance;
	}

	TexturePtr TextureBuilder::BuildTexture(const std::string& path)
	{
		auto it = _texturesCache.find(path);
		if (it != _texturesCache.end())
			return it->second;

		auto texture = std::make_shared<Texture>(_device,
			path,
			_commandBuffer,
			_uploadContext,
			_queue,
			VK_IMAGE_ASPECT_COLOR_BIT);
		_texturesCache[path] = texture;
		return texture;
	}
}