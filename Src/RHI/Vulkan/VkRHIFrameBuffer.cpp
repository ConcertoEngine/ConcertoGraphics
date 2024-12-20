//
// Created by arthur on 15/05/2024.
//

#include <vector>

#include <Concerto/Core/Cast.hpp>
#include "Concerto/Graphics/RHI/Vulkan/VkRHIFrameBuffer.hpp"

#include "Concerto/Graphics/RHI/Vulkan/VKRHITexture.hpp"
#include "Concerto/Graphics/RHI/Vulkan/VkRHIDevice.hpp"
#include "Concerto/Graphics/RHI/Vulkan/VKRHIRenderPass.hpp"
#include "Concerto/Graphics/Backend/Vulkan/Wrapper/ImageView.hpp"

namespace cct::gfx::rhi
{
	namespace //Fixme
	{
		std::vector<VkImageView> ToImageView(const std::vector<std::unique_ptr<Texture>>& textures)
		{
			std::vector<VkImageView> imageViews;
			for (auto& texture : textures)
				imageViews.emplace_back(*Cast<const VkRHITexture&>(*texture).GetImageView().Get());
			return imageViews;
		}

		std::vector<VkImageView> ToImageView(const std::vector<std::unique_ptr<TextureView>>& textures)
		{
			std::vector<VkImageView> imageViews;
			for (auto& texture : textures)
				imageViews.emplace_back(*Cast<const VkRHITextureView&>(*texture).GetImageView().Get());
			return imageViews;
		}
	}

	VkRHIFrameBuffer::VkRHIFrameBuffer(VkRHIDevice& device, UInt32 width, UInt32 height, const VkRHIRenderPass& renderPass, const std::vector<std::unique_ptr<rhi::Texture>>& attachments) :
		rhi::FrameBuffer(),
		vk::FrameBuffer(device, renderPass, ToImageView(attachments), { width, height })
	{
	}

	VkRHIFrameBuffer::VkRHIFrameBuffer(VkRHIDevice& device, UInt32 width, UInt32 height, const VkRHIRenderPass& renderPass, const std::vector<std::unique_ptr<rhi::TextureView>>& attachments) :
		rhi::FrameBuffer(),
		vk::FrameBuffer(device, renderPass, ToImageView(attachments), { width, height })
	{
	}

	UInt32 VkRHIFrameBuffer::GetWidth() const
	{
		return GetExtent2D().width;
	}

	UInt32 VkRHIFrameBuffer::GetHeight() const
	{
		return GetExtent2D().height;
	}
}
