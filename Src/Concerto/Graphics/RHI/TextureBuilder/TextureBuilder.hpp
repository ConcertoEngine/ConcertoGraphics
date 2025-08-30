//
// Created by arthur on 07/08/2024.
//

#ifndef CONCERTO_GRAPHICS_RHI_TEXTUREBUILDER_HPP
#define CONCERTO_GRAPHICS_RHI_TEXTUREBUILDER_HPP

#include <memory>
#include <vector>
#include <string>
#include <unordered_map>

#include "Concerto/Graphics/RHI/Defines.hpp"
#include "Concerto/Graphics/RHI/Texture.hpp"

namespace cct::gfx::rhi
{
	class Buffer;
	class Device;

	class CONCERTO_GRAPHICS_RHI_BASE_API TextureBuilder
	{
	public:
		TextureBuilder(Device& device);
		virtual ~TextureBuilder() = default;

		TextureBuilder(TextureBuilder&&) = default;
		TextureBuilder(const TextureBuilder&) = delete;

		TextureBuilder& operator=(TextureBuilder&&) = default;
		TextureBuilder& operator=(const TextureBuilder&) = delete;

		std::shared_ptr<Texture> BuildTexture(const std::string& path);
		void Commit();
	protected:
		virtual std::shared_ptr<Texture> BuildApiTexture(PixelFormat format, Int32 width, Int32 height) = 0;
		virtual void InternalCommit() = 0;
	private:
		Device& m_device;
		ThreadSafeHashMap<std::string, std::shared_ptr<Texture>> m_texturesCache;
		ThreadSafeHashMap<std::shared_ptr<Texture>, std::unique_ptr<Buffer>> m_buffersToUpload;
	};
}

#endif //CONCERTO_GRAPHICS_RHI_TEXTUREBUILDER_HPP