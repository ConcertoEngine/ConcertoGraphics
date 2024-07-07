//
// Created by arthur on 12/05/2024.
//

#ifndef CONCERTO_GRAPHICS_RHI_DEVICE_HPP
#define CONCERTO_GRAPHICS_RHI_DEVICE_HPP

#include <iosfwd>
#include <iosfwd>
#include <string>
#include <memory>
#include <span>
#include <vector>
#include <vector>

#include "FrameBuffer.hpp"
#include "Texture.hpp"
#include "Concerto/Graphics/Defines.hpp"
#include "Concerto/Graphics/RHI/Enums.hpp"
#include "Concerto/Graphics/RHI/RenderPass.hpp"

namespace Concerto::Graphics
{
	class Window;
}

namespace Concerto::Graphics::RHI
{
	class SwapChain;

	struct DeviceInfo
	{
		std::string name;
		UInt32 vendor;
		DeviceType type;
	};

	class CONCERTO_GRAPHICS_API Device
	{
	public:
		Device() = default;
		virtual ~Device() = default;
		virtual std::unique_ptr<SwapChain> CreateSwapChain(Window& window, PixelFormat pixelFormat = PixelFormat::BGRA8_SRGB, PixelFormat depthPixelFormat = PixelFormat::D32f) = 0;
		virtual std::unique_ptr<RenderPass> CreateRenderPass(std::span<RenderPass::Attachment> attachments, std::span<RenderPass::SubPassDescription> subPassDescriptions, std::span<RenderPass::SubPassDependency> subPassDependencies) = 0;
    virtual std::unique_ptr<FrameBuffer> CreateFrameBuffer(UInt32 width, UInt32 height, const RenderPass& renderPass, const std::vector<Texture>& attachments) = 0;
  };
}

#endif //CONCERTO_GRAPHICS_RHI_DEVICE_HPP