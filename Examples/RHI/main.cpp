//
// Created by arthur on 12/07/2022.
//


#include <Concerto/Graphics/Window/Window.hpp>
#include <Concerto/Graphics/RHI/APIImpl.hpp>
#include <Concerto/Graphics/RHI/Instance.hpp>
#include <Concerto/Graphics/RHI/SwapChain.hpp>
#include <Concerto/Graphics/RHI/RenderPass.hpp>
#include <Concerto/Graphics/RHI/FrameBuffer.hpp>
#include <Concerto/Graphics/RHI/Texture.hpp>

using namespace Concerto;
using namespace Concerto::Graphics;

int main()
{
	try
	{
		Window window("Concerto Graphics", 1280, 720);
		Input& inputManager = window.GetInputManager();

		RHI::Instance rInstance;
		std::unique_ptr<RHI::Device> device;

		std::size_t deviceIndex = 0;
		for (const auto& [name, vendor, type] : rInstance.EnumerateDevices())
		{
			if (type == RHI::DeviceType::Dedicated)
			{
				device = rInstance.CreateDevice(deviceIndex);
				break;
			}
			++deviceIndex;
		}
		auto swapChain = device->CreateSwapChain(window);

		std::vector<RHI::RenderPass::Attachment> attachment;
		auto& colorAttachment = attachment.emplace_back();
		colorAttachment.pixelFormat = swapChain->GetPixelFormat();
		colorAttachment.loadOp = RHI::AttachmentLoadOp::Clear;
		colorAttachment.storeOp = RHI::AttachmentStoreOp::Store;
		colorAttachment.stencilLoadOp = RHI::AttachmentLoadOp::DontCare;
		colorAttachment.stencilStoreOp = RHI::AttachmentStoreOp::DontCare;
		colorAttachment.initialLayout = RHI::ImageLayout::Undefined;
		colorAttachment.finalLayout = RHI::ImageLayout::PresentSrcKhr;

		auto& depthAttachment = attachment.emplace_back();
		depthAttachment.pixelFormat = swapChain->GetDepthPixelFormat();
		depthAttachment.loadOp = RHI::AttachmentLoadOp::Clear;
		depthAttachment.storeOp = RHI::AttachmentStoreOp::Store;
		depthAttachment.stencilLoadOp = RHI::AttachmentLoadOp::Clear;
		depthAttachment.stencilStoreOp = RHI::AttachmentStoreOp::DontCare;
		depthAttachment.initialLayout = RHI::ImageLayout::Undefined;
		depthAttachment.finalLayout = RHI::ImageLayout::DepthStencilAttachmentOptimal;

		std::vector<RHI::RenderPass::SubPassDescription> subPassDescriptions;

		auto& subPass = subPassDescriptions.emplace_back();
		subPass.colorAttachments.push_back({0, RHI::ImageLayout::ColorAttachmentOptimal});
		subPass.depthStencilAttachment = {1, RHI::ImageLayout::DepthStencilAttachmentOptimal};


		std::vector<RHI::RenderPass::SubPassDependency> subPassDependencies;
		auto& colorDependency = subPassDependencies.emplace_back();
		colorDependency.srcSubPassIndex = RHI::RenderPass::SubPassDependency::ExternalSubPass;
		colorDependency.dstSubPassIndex = 0u;
		colorDependency.srcStageMask = static_cast<RHI::PipelineStageFlags>(RHI::PipelineStage::ColorAttachmentOutput);
		colorDependency.srcAccessFlags = 0u;
		colorDependency.dstStageMask = static_cast<RHI::PipelineStageFlags>(RHI::PipelineStage::ColorAttachmentOutput);
		colorDependency.dstAccessFlags = static_cast<RHI::MemoryAccessFlags>(RHI::MemoryAccess::ColorAttachmentWrite);

		auto& depthDependency = subPassDependencies.emplace_back();
		depthDependency.srcSubPassIndex = RHI::RenderPass::SubPassDependency::ExternalSubPass;
		depthDependency.dstSubPassIndex = 0u;
		depthDependency.srcStageMask = static_cast<RHI::PipelineStageFlags>(RHI::PipelineStage::EarlyFragmentTests) | static_cast<RHI::PipelineStageFlags>(RHI::PipelineStage::LateFragmentTests);
		depthDependency.srcAccessFlags = 0u;
		depthDependency.dstStageMask = static_cast<RHI::PipelineStageFlags>(RHI::PipelineStage::EarlyFragmentTests) | static_cast<RHI::PipelineStageFlags>(RHI::PipelineStage::LateFragmentTests);
		depthDependency.dstAccessFlags = static_cast<RHI::MemoryAccessFlags>(RHI::MemoryAccess::DepthStencilAttachmentWrite);

		auto renderPass = device->CreateRenderPass(attachment, subPassDescriptions, subPassDependencies);

		std::vector<RHI::Texture> attachments;
		auto frameBuffer = device->CreateFrameBuffer(window.GetWidth(), window.GetHeight(), *renderPass, attachments);

		while (!window.ShouldClose())
		{
		}
	}
	catch (const std::exception& e)
	{
		std::cout << e.what() << std::endl;
	}
	return 0;
}
