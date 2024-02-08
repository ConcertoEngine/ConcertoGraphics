//
// Created by arthur on 17/02/2023.
//

#ifndef CONCERTO_GRAPHICS_INCLUDE_TEXTUREBUILDER_HPP_
#define CONCERTO_GRAPHICS_INCLUDE_TEXTUREBUILDER_HPP_

#include <unordered_map>
#include <memory>
#include <string>

#include "Concerto/Graphics/Defines.hpp"


namespace Concerto::Graphics
{
	class UploadContext;
	class Device;
	class Allocator;
	class CommandBuffer;
	class Queue;
	class Texture;

	class CONCERTO_GRAPHICS_API TextureBuilder
	{
	 public:
		TextureBuilder(Device& device, Allocator& allocator, CommandBuffer& commandBuffer, UploadContext& uploadContext, Queue& queue);
		~TextureBuilder();

		static TextureBuilder* Instance();

		std::shared_ptr<Texture> BuildTexture(const std::string& path);
	 private:
		static TextureBuilder* _instance;
		std::unordered_map<std::string, std::shared_ptr<Texture>> _texturesCache;
		Device& _device;
		CommandBuffer& _commandBuffer;
		UploadContext& _uploadContext;
		Queue& _queue;
	};
}

#endif //CONCERTO_GRAPHICS_INCLUDE_TEXTUREBUILDER_HPP_
