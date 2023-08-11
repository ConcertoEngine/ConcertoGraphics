//
// Created by arthur on 17/02/2023.
//

#ifndef CONCERTOGRAPHICS_INCLUDE_TEXTUREBUILDER_HPP_
#define CONCERTOGRAPHICS_INCLUDE_TEXTUREBUILDER_HPP_

#include <unordered_map>

#include <Concerto/Core/Types.hpp>

#include "Concerto/Graphics/Vulkan/Texture.hpp"

namespace Concerto::Graphics
{
	class UploadContext;
	namespace Wrapper{
		class Device;
		class Allocator;
		class CommandBuffer;
		class Queue;
	}
	class CONCERTO_PUBLIC_API TextureBuilder
	{
	 public:
		TextureBuilder(Device& device, Allocator& allocator, CommandBuffer& commandBuffer,
					   UploadContext& uploadContext, Queue& queue);

		TexturePtr BuildTexture(const std::string& path);
	 private:
		std::unordered_map<std::string, TexturePtr> _texturesCache;
		Device& _device;
		Allocator& _allocator;
		CommandBuffer& _commandBuffer;
		UploadContext& _uploadContext;
		Queue& _queue;
	};
}

#endif //CONCERTOGRAPHICS_INCLUDE_TEXTUREBUILDER_HPP_
