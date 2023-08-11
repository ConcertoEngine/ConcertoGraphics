//
// Created by arthur on 17/02/2023.
//

#include "TextureBuilder.hpp"
#include "VulkanRenderer.hpp"

namespace Concerto::Graphics
{
	TextureBuilder::TextureBuilder(Device& device, Allocator& allocator,
		CommandBuffer& commandBuffer, UploadContext& uploadContext, Queue& queue) :
		_device(device),
		_allocator(allocator),
		_commandBuffer(commandBuffer),
		_uploadContext(uploadContext),
		_queue(queue)
	{

	}

	TexturePtr TextureBuilder::BuildTexture(const std::string& path)
	{
		auto it = _texturesCache.find(path);
		if (it != _texturesCache.end())
			return it->second;
		auto texture = std::make_shared<Texture>(_device,
			path,
			_allocator,
			_commandBuffer,
			_uploadContext,
			_queue,
			VK_IMAGE_ASPECT_COLOR_BIT);
		_texturesCache[path] = texture;
		return texture;
	}
}