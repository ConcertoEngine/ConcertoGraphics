//
// Created by arthur on 17/02/2023.
//

#ifndef CONCERTO_GRAPHICS_INCLUDE_TEXTUREBUILDER_HPP_
#define CONCERTO_GRAPHICS_INCLUDE_TEXTUREBUILDER_HPP_

#include <unordered_map>
#include <memory>
#include <string>

#include "Concerto/Graphics/Defines.hpp"
#include "Concerto/Graphics/RHI/TextureBuilder/TextureBuilder.hpp"

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

	protected:
		std::shared_ptr<Texture> BuildApiTexture(PixelFormat format, Int32 width, Int32 height) override;
		void InternalCommit() override;
	 private:
		static VkRHITextureBuilder* m_instance;
		rhi::Device& m_device;
		vk::UploadContext& m_uploadContext;
		vk::Queue& m_queue;
	};
}

#endif //CONCERTO_GRAPHICS_INCLUDE_TEXTUREBUILDER_HPP_
