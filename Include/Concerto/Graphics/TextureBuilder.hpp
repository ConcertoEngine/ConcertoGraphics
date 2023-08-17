//
// Created by arthur on 17/02/2023.
//

#ifndef CONCERTOGRAPHICS_INCLUDE_TEXTUREBUILDER_HPP_
#define CONCERTOGRAPHICS_INCLUDE_TEXTUREBUILDER_HPP_

#include <unordered_map>
#include <memory>
#include <string>

#include <Concerto/Core/Types.hpp>


namespace Concerto::Graphics
{
	class UploadContext;
	class Device;
	class Allocator;
	class CommandBuffer;
	class Queue;
	class Texture;

	class CONCERTO_PUBLIC_API TextureBuilder
	{
	 public:
		TextureBuilder(Device& device, Allocator& allocator, CommandBuffer& commandBuffer, UploadContext& uploadContext, Queue& queue);

		std::shared_ptr<Texture> BuildTexture(const std::string& path);
	 private:
		std::unordered_map<std::string, std::shared_ptr<Texture>> _texturesCache;
		Device& _device;
		CommandBuffer& _commandBuffer;
		UploadContext& _uploadContext;
		Queue& _queue;
	};
}

#endif //CONCERTOGRAPHICS_INCLUDE_TEXTUREBUILDER_HPP_
