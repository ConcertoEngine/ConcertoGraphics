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

namespace cct::gfx::vk
{
	class UploadContext;
	class CommandBuffer;
	class Queue;
}

namespace cct::gfx::rhi
{
	class Device;

	class CONCERTO_GRAPHICS_RHI_BASE_API VkRHITextureBuilder : public rhi::TextureBuilder
	{
	 public:
		VkRHITextureBuilder(rhi::Device& device, vk::UploadContext& uploadContext, vk::Queue& queue);
		~VkRHITextureBuilder() override;

		static VkRHITextureBuilder* Instance();

		std::shared_ptr<rhi::Texture> BuildTexture(const std::string& path) override;
	 private:
		static VkRHITextureBuilder* _instance;
		std::unordered_map<std::string, std::shared_ptr<Texture>> _texturesCache;
		rhi::Device& _device;
		vk::UploadContext& _uploadContext;
		vk::Queue& _queue;
	};
}

#endif //CONCERTO_GRAPHICS_INCLUDE_TEXTUREBUILDER_HPP_
