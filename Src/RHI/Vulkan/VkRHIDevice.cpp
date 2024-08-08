//
// Created by arthur on 15/05/2024.
//

#include <Concerto/Core/Cast.hpp>

#include "Concerto/Graphics/RHI/Vulkan/VkRHIDevice.hpp"

#include "Concerto/Graphics/Backend/Vulkan/Wrapper/PhysicalDevice.hpp"
#include "Concerto/Graphics/RHI/SwapChain.hpp"
#include "Concerto/Graphics/Window/Window.hpp"
#include "Concerto/Graphics/RHI/Vulkan/VkRHISwapChain.hpp"
#include "Concerto/Graphics/RHI/Vulkan/VKRHIRenderPass.hpp"
#include "Concerto/Graphics/RHI/Vulkan/VKRHIFrameBuffer.hpp"
#include "Concerto/Graphics/RHI/Vulkan/Utils.hpp"
#include "Concerto/Graphics/RHI/Vulkan/VkRHIMaterialBuilder.hpp"
#include "Concerto/Graphics/RHI/Vulkan/VkRHITextureBuilder.hpp"

namespace Concerto::Graphics::RHI
{
	VkRHIDevice::VkRHIDevice(Vk::PhysicalDevice& physicalDevice, Vk::Instance& instance) :
		Vk::Device(physicalDevice, instance),
		_surface(nullptr),
		_vkInstance(&instance)
	{
	}

	std::unique_ptr<SwapChain> VkRHIDevice::CreateSwapChain(Window& window, PixelFormat pixelFormat, PixelFormat depthPixelFormat)
	{
		CONCERTO_ASSERT(_vkInstance, "ConcertoGraphics: Invalid Vulkan instance");
		if (GetPhysicalDevice().GetSurface() == nullptr)
		{
			VkSurfaceKHR surface;
			window.CreateVulkanSurface(*_vkInstance, &surface);
			GetPhysicalDevice().SetSurface(surface);
		}
		auto swapChain = std::make_unique<VkRHISwapChain>(*this, window, pixelFormat, depthPixelFormat);
		if (swapChain->GetLastResult() != VK_SUCCESS)
		{
			CONCERTO_ASSERT_FALSE("ConcertoGraphics: Error occured during swapchain creation error={}", static_cast<Int32>(swapChain->GetLastResult()));
			return nullptr;
		}
		return swapChain;
	}

	std::unique_ptr<RenderPass> VkRHIDevice::CreateRenderPass(std::span<RHI::RenderPass::Attachment> attachments, std::span<RHI::RenderPass::SubPassDescription> subPassDescriptions, std::span<RHI::RenderPass::SubPassDependency> subPassDependencies)
	{
		std::vector<VkAttachmentDescription> vkAttachmentDescriptions;
		std::vector<VkSubpassDescription> vkSubPassDescriptions;
		std::vector<VkSubpassDependency> vkSubPassDependencies;
		vkAttachmentDescriptions.reserve(attachments.size());
		vkSubPassDescriptions.reserve(subPassDescriptions.size());
		vkSubPassDependencies.reserve(subPassDependencies.size());

		for (const auto& attachment : attachments)
		{
			auto& vkAttachment = vkAttachmentDescriptions.emplace_back();
			vkAttachment.format = Converters::ToVulkan(attachment.pixelFormat);
			vkAttachment.loadOp = Converters::ToVulkan(attachment.loadOp);
			vkAttachment.stencilLoadOp = Converters::ToVulkan(attachment.stencilLoadOp);
			vkAttachment.storeOp = Converters::ToVulkan(attachment.storeOp);
			vkAttachment.stencilStoreOp = Converters::ToVulkan(attachment.stencilStoreOp);
			vkAttachment.finalLayout = Converters::ToVulkan(attachment.finalLayout);
			vkAttachment.finalLayout = Converters::ToVulkan(attachment.finalLayout);
			vkAttachment.samples = VK_SAMPLE_COUNT_1_BIT;
		}

		std::vector<std::vector<VkAttachmentReference>> vkInputAttachmentReferences(subPassDescriptions.size());
		std::vector<std::vector<VkAttachmentReference>> vkColorAttachmentReferences(subPassDescriptions.size());
		std::vector<VkAttachmentReference> vkDepthStencilAttachment(subPassDescriptions.size());

		std::size_t i = 0;
		for (auto& subPassDescription : subPassDescriptions)
		{
			auto& vkSubPassDesc = vkSubPassDescriptions.emplace_back();

			//Input attachment
			vkInputAttachmentReferences[i].reserve(subPassDescription.inputAttachments.size());
			for (const auto& attachmentReference : subPassDescription.inputAttachments)
				vkInputAttachmentReferences[i].emplace_back(attachmentReference.attachmentIndex, Converters::ToVulkan(attachmentReference.imageLayout));
			vkSubPassDesc.inputAttachmentCount = static_cast<UInt32>(subPassDescription.inputAttachments.size());
			vkSubPassDesc.pInputAttachments = vkInputAttachmentReferences[i].data();

			//Color attachment
			vkColorAttachmentReferences[i].reserve(subPassDescription.colorAttachments.size());
			for (const auto& attachmentReference : subPassDescription.colorAttachments)
				vkColorAttachmentReferences[i].emplace_back(attachmentReference.attachmentIndex, Converters::ToVulkan(attachmentReference.imageLayout));
			vkSubPassDesc.colorAttachmentCount = static_cast<UInt32>(subPassDescription.colorAttachments.size());
			vkSubPassDesc.pColorAttachments = vkColorAttachmentReferences[i].data();

			//DepthStencil attachment
			if (subPassDescription.depthStencilAttachment)
			{
				vkDepthStencilAttachment[i] = { subPassDescription.depthStencilAttachment->attachmentIndex, Converters::ToVulkan(subPassDescription.depthStencilAttachment->imageLayout) };
				vkSubPassDesc.pDepthStencilAttachment = &vkDepthStencilAttachment[i];
			}

			//Preserve attachment
			vkSubPassDesc.preserveAttachmentCount = static_cast<UInt32>(subPassDescription.preserveAttachments.size());
			vkSubPassDesc.pPreserveAttachments = subPassDescription.preserveAttachments.data();
			++i;
		}

		for (const auto& subPassDependency : subPassDependencies)
		{
			auto& vkSubPassDependency = vkSubPassDependencies.emplace_back();
			vkSubPassDependency.srcSubpass = subPassDependency.srcSubPassIndex;
			vkSubPassDependency.srcStageMask = Converters::ToVulkan<VkPipelineStageFlagBits>(subPassDependency.srcStageMask);
			vkSubPassDependency.srcAccessMask = Converters::ToVulkan<VkAccessFlagBits>(subPassDependency.srcAccessFlags);
			vkSubPassDependency.dstSubpass = subPassDependency.dstSubPassIndex;
			vkSubPassDependency.dstStageMask = Converters::ToVulkan<VkPipelineStageFlagBits>(subPassDependency.dstStageMask);
			vkSubPassDependency.dstAccessMask = Converters::ToVulkan<VkAccessFlagBits>(subPassDependency.dstAccessFlags);
		}

		return std::make_unique<VkRHIRenderPass>(*this, vkAttachmentDescriptions, vkSubPassDescriptions, vkSubPassDependencies);
	}

	std::unique_ptr<FrameBuffer> VkRHIDevice::CreateFrameBuffer(UInt32 width, UInt32 height, const RHI::RenderPass& renderPass, const std::vector<RHI::Texture>& attachments)
	{
		return std::make_unique<VkRHIFrameBuffer>(*this, width, height, Cast<const VkRHIRenderPass&>(renderPass), attachments);
	}

	std::unique_ptr<MaterialBuilder> VkRHIDevice::CreateMaterialBuilder(const Vector2u& windowExtent)
	{
		return std::make_unique<VkRHIMaterialBuilder>(*this, VkExtent2D{ windowExtent.X(), windowExtent.Y() });
	}

	std::unique_ptr<TextureBuilder> VkRHIDevice::CreateTextureBuilder()
	{
		auto& uploadContext = GetUploadContext();
		return std::make_unique<VkRHITextureBuilder>(*this, uploadContext, GetQueue(Vk::Queue::Type::Graphics));
	}

	Vk::UploadContext& VkRHIDevice::GetUploadContext()
	{
		if (_uploadContext.has_value() == false)
			_uploadContext.emplace(*this, GetQueueFamilyIndex(Vk::Queue::Type::Graphics));
		return _uploadContext.value();
	}
} //Concerto::Graphics::RHI
