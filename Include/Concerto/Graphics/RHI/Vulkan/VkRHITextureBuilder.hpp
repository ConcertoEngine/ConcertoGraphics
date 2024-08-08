//
// Created by arthur on 17/02/2023.
//

#ifndef CONCERTO_GRAPHICS_INCLUDE_TEXTUREBUILDER_HPP_
#define CONCERTO_GRAPHICS_INCLUDE_TEXTUREBUILDER_HPP_

#include <unordered_map>
#include <memory>
#include <string>

#include "Concerto/Graphics/Defines.hpp"
#include "Concerto/Graphics/RHI/TextureBuilder.hpp"

namespace Concerto::Graphics::Vk
{
	class UploadContext;
	class CommandBuffer;
	class Queue;
}

namespace Concerto::Graphics::RHI
{
	class Device;

	class CONCERTO_GRAPHICS_API VkRHITextureBuilder : public RHI::TextureBuilder
	{
	 public:
		VkRHITextureBuilder(RHI::Device& device, Vk::UploadContext& uploadContext, Vk::Queue& queue);
		~VkRHITextureBuilder() override;

		static VkRHITextureBuilder* Instance();

		std::shared_ptr<RHI::Texture> BuildTexture(const std::string& path) override;
	 private:
		static VkRHITextureBuilder* _instance;
		std::unordered_map<std::string, std::shared_ptr<Texture>> _texturesCache;
		RHI::Device& _device;
		Vk::UploadContext& _uploadContext;
		Vk::Queue& _queue;
	};
}

#endif //CONCERTO_GRAPHICS_INCLUDE_TEXTUREBUILDER_HPP_
