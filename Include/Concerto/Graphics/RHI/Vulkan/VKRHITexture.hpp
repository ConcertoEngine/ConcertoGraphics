//
// Created by arthur on 12/05/2024.
//

#ifndef CONCERTO_GRAPHICS_RHI_VULKAN_VKRHITEXTURE_HPP
#define CONCERTO_GRAPHICS_RHI_VULKAN_VKRHITEXTURE_HPP

#include "Concerto/Graphics/Defines.hpp"
#include "Concerto/Graphics/RHI/Texture.hpp"
#include "Concerto/Graphics/Backend/Vulkan/Wrapper/ImageView.hpp"

namespace Concerto::Graphics::RHI
{
	class VkRHIDevice;
	class RenderPass;
	class VkRHIRenderPass;

	class CONCERTO_GRAPHICS_API VkRHITexture final : public Texture, public Vk::ImageView
	{
	public:
		VkRHITexture() = default;
	private:
	};
}

#endif //CONCERTO_GRAPHICS_RHI_VULKAN_VKRHITEXTURE_HPP