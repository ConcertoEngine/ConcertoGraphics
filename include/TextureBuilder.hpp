//
// Created by arthur on 17/02/2023.
//

#ifndef CONCERTOGRAPHICS_INCLUDE_TEXTUREBUILDER_HPP_
#define CONCERTOGRAPHICS_INCLUDE_TEXTUREBUILDER_HPP_

#include <unordered_map>
#include "Vulkan/Texture.hpp"

namespace Concerto::Graphics
{
	class UploadContext;
	namespace Wrapper{
		class Device;
		class Allocator;
		class CommandBuffer;
		class Queue;
	}
	class TextureBuilder
	{
	 public:
		TextureBuilder(Wrapper::Device& device, Wrapper::Allocator& allocator, Wrapper::CommandBuffer& commandBuffer,
					   UploadContext& uploadContext, Wrapper::Queue& queue);

		TexturePtr BuildTexture(const std::string& path);
	 private:
		std::unordered_map<std::string, TexturePtr> _texturesCache;
		Wrapper::Device& _device;
		Wrapper::Allocator& _allocator;
		Wrapper::CommandBuffer& _commandBuffer;
		UploadContext& _uploadContext;
		Wrapper::Queue& _queue;
	};
}

#endif //CONCERTOGRAPHICS_INCLUDE_TEXTUREBUILDER_HPP_
