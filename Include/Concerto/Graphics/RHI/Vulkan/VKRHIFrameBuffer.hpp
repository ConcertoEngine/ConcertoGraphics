//
// Created by arthur on 12/05/2024.
//

#ifndef CONCERTO_GRAPHICS_BACKEND_RHI_VULKAN_FRAMEBUFFER_HPP
#define CONCERTO_GRAPHICS_BACKEND_RHI_VULKAN_FRAMEBUFFER_HPP

#include "Concerto/Graphics/Defines.hpp"
#include "Concerto/Graphics/RHI/Texture.hpp"
#include "Concerto/Graphics/RHI/FrameBuffer.hpp"
#include "Concerto/Graphics/Backend/Vulkan/Wrapper/FrameBuffer.hpp"

namespace Concerto::Graphics::RHI
{
	class VkRHIDevice;
	class VkRHIRenderPass;

	class CONCERTO_GRAPHICS_RHI_BASE_API VkRHIFrameBuffer final : public FrameBuffer, public Vk::FrameBuffer
	{
	public:
		VkRHIFrameBuffer(VkRHIDevice& device, UInt32 width, UInt32 height, const VkRHIRenderPass& renderPass, const std::vector<std::unique_ptr<RHI::Texture>>& attachments);
		VkRHIFrameBuffer(VkRHIDevice& device, UInt32 width, UInt32 height, const VkRHIRenderPass& renderPass, const std::vector<std::unique_ptr<RHI::TextureView>>& attachments);
		UInt32 GetWidth() const override;
		UInt32 GetHeight() const override;
	};
}

#endif //CONCERTO_GRAPHICS_BACKEND_RHI_VULKAN_FRAMEBUFFER_HPP