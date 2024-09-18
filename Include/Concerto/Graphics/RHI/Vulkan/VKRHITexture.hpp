//
// Created by arthur on 12/05/2024.
//

#ifndef CONCERTO_GRAPHICS_RHI_VULKAN_VKRHITEXTURE_HPP
#define CONCERTO_GRAPHICS_RHI_VULKAN_VKRHITEXTURE_HPP

#include "Concerto/Graphics/RHI/Defines.hpp"
#include "Concerto/Graphics/Backend/Vulkan/Wrapper/Image.hpp"
#include "Concerto/Graphics/RHI/Texture.hpp"
#include "Concerto/Graphics/Backend/Vulkan/Wrapper/ImageView.hpp"

namespace Concerto::Graphics::Vk
{
	class Device;
	class CommandBuffer;
	class UploadContext;
	class Queue;
}

namespace Concerto::Graphics::RHI
{
	class CONCERTO_GRAPHICS_RHI_BASE_API VkRHITexture : public RHI::Texture
	{
	public:
		VkRHITexture(Vk::Device& device,
			const std::string& file,
			Vk::CommandBuffer& commandBuffer,
			Vk::UploadContext& uploadContext,
			Vk::Queue& queue,
			VkImageAspectFlags aspectFlags);

		const Vk::Image& GetImage() const;
		const Vk::ImageView& GetImageView() const;

	private:
		Vk::Image _image;
		Vk::ImageView _imageView;
	};
	using VkRHITexturePtr = std::shared_ptr<VkRHITexture>;

	class CONCERTO_GRAPHICS_RHI_BASE_API VkRHITextureView : public RHI::TextureView
	{
	public:
		VkRHITextureView(const Vk::ImageView& image);
		const Vk::ImageView& GetImageView() const;
	private:
		const Vk::ImageView* _imageView;
	};
}

#endif //CONCERTO_GRAPHICS_RHI_VULKAN_VKRHITEXTURE_HPP