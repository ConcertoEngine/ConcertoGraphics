//
// Created by arthur on 12/05/2024.
//

#ifndef CONCERTO_GRAPHICS_RHI_VULKAN_VKRHITEXTURE_HPP
#define CONCERTO_GRAPHICS_RHI_VULKAN_VKRHITEXTURE_HPP

#include <memory>

#include "Concerto/Graphics/RHI/Defines.hpp"
#include "Concerto/Graphics/Backend/Vulkan/Wrapper/Image.hpp"
#include "Concerto/Graphics/RHI/Texture.hpp"
#include "Concerto/Graphics/Backend/Vulkan/Wrapper/ImageView.hpp"

namespace cct::gfx::vk
{
	class Device;
	class CommandBuffer;
	class UploadContext;
	class Queue;
}

namespace cct::gfx::rhi
{
	class CONCERTO_GRAPHICS_RHI_BASE_API VkRHITexture : public rhi::Texture
	{
	public:
		VkRHITexture(vk::Device& device,
			const std::string& file,
			vk::CommandBuffer& commandBuffer,
			vk::UploadContext& uploadContext,
			vk::Queue& queue,
			VkImageAspectFlags aspectFlags);

		const vk::Image& GetImage() const;
		const vk::ImageView& GetImageView() const;

	private:
		vk::Image _image;
		vk::ImageView _imageView;
	};
	using VkRHITexturePtr = std::shared_ptr<VkRHITexture>;

	class CONCERTO_GRAPHICS_RHI_BASE_API VkRHITextureView : public rhi::TextureView
	{
	public:
		VkRHITextureView(const vk::ImageView& image);
		const vk::ImageView& GetImageView() const;
	private:
		const vk::ImageView* _imageView;
	};
}

#endif //CONCERTO_GRAPHICS_RHI_VULKAN_VKRHITEXTURE_HPP