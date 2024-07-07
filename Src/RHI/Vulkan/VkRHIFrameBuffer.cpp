//
// Created by arthur on 15/05/2024.
//

#include <vector>
#include <span>

#include "Concerto/Graphics/RHI/Vulkan/VkRHIFrameBuffer.hpp"

#include "Concerto/Graphics/RHI/Vulkan/VKRHITexture.hpp"
#include "Concerto/Graphics/RHI/Vulkan/VkRHIDevice.hpp"
#include "Concerto/Graphics/RHI/Vulkan/VKRHIRenderPass.hpp"
#include "Concerto/Graphics/Backend/Vulkan/Wrapper/ImageView.hpp"

namespace Concerto::Graphics::RHI
{
	namespace //Fixme
	{
		std::vector<VkImageView> ToImageView(const std::vector<Texture>& textures)
		{
			std::vector<VkImageView> imageViews(textures.size());
			for (auto& texture : textures)
				imageViews.emplace_back(*static_cast<const VkRHITexture&>(texture).Get());
			return imageViews;
		}
	}

    VkRHIFrameBuffer::VkRHIFrameBuffer(VkRHIDevice& device, UInt32 width, UInt32 height, const VkRHIRenderPass& renderPass, const std::vector<Texture>& attachments) :
      RHI::FrameBuffer(),
      Vk::FrameBuffer(device, renderPass, ToImageView(attachments), {width, height})
    {
    }
}
