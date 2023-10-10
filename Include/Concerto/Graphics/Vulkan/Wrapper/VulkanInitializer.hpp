//
// Created by arthur on 24/03/2022.
//

#ifndef ENGINE_VULKANINITIALIZER_HPP
#define ENGINE_VULKANINITIALIZER_HPP

#include <vulkan/vulkan.hpp>
#include "Concerto/Graphics/Defines.hpp"

namespace VulkanInitializer
{
	CONCERTO_PUBLIC_API VkPipelineShaderStageCreateInfo PipelineShaderStageCreateInfo(VkShaderStageFlagBits stage, VkShaderModule shaderModule);
	CONCERTO_PUBLIC_API VkPipelineVertexInputStateCreateInfo VertexInputStateCreateInfo();
	CONCERTO_PUBLIC_API VkPipelineInputAssemblyStateCreateInfo InputAssemblyCreateInfo(VkPrimitiveTopology topology);
	CONCERTO_PUBLIC_API VkPipelineRasterizationStateCreateInfo RasterizationStateCreateInfo(VkPolygonMode polygonMode);
	CONCERTO_PUBLIC_API VkPipelineMultisampleStateCreateInfo MultisamplingStateCreateInfo();
	CONCERTO_PUBLIC_API VkPipelineColorBlendAttachmentState ColorBlendAttachmentState();
	CONCERTO_PUBLIC_API VkPipelineLayoutCreateInfo PipelineLayoutCreateInfo();
	CONCERTO_PUBLIC_API VkRenderPassBeginInfo RenderPassBeginInfo(VkRenderPass pT, VkExtent2D extent2D, VkFramebuffer framebuffer);
	CONCERTO_PUBLIC_API VkImageCreateInfo ImageCreateInfo(VkFormat format, VkImageUsageFlags usageFlags, VkExtent3D extent);
	CONCERTO_PUBLIC_API VkImageViewCreateInfo ImageViewCreateInfo(VkFormat format, VkImage image, VkImageAspectFlags aspectFlags);
	CONCERTO_PUBLIC_API VkPipelineDepthStencilStateCreateInfo DepthStencilCreateInfo(bool bDepthTest, bool bDepthWrite, VkCompareOp compareOp);
	CONCERTO_PUBLIC_API VkCommandBufferBeginInfo CommandBufferBeginInfo(VkCommandBufferUsageFlagBits bits);
	CONCERTO_PUBLIC_API VkSubmitInfo SubmitInfo(VkCommandBuffer* pT);
	CONCERTO_PUBLIC_API VkPresentInfoKHR PresentInfo();
	CONCERTO_PUBLIC_API VkCommandPoolCreateInfo CommandPoolCreateInfo(uint32_t queueFamilyIndex, VkCommandPoolCreateFlags flags);
	CONCERTO_PUBLIC_API VkFenceCreateInfo FenceCreateInfo(VkFenceCreateFlagBits bits = static_cast<VkFenceCreateFlagBits>(0));
	CONCERTO_PUBLIC_API VkSemaphoreCreateInfo SemaphoreCreateInfo(VkSemaphoreCreateFlags flags = 0);
	CONCERTO_PUBLIC_API VkCommandBufferAllocateInfo CommandBufferAllocateInfo(VkCommandPool pool, int count, VkCommandBufferLevel level = VK_COMMAND_BUFFER_LEVEL_PRIMARY);
	CONCERTO_PUBLIC_API VkFramebufferCreateInfo FramebufferCreateInfo(VkRenderPass pT, VkExtent2D extent2D);
	CONCERTO_PUBLIC_API VkDescriptorSetLayoutBinding DescriptorSetLayoutBinding(VkDescriptorType type, VkShaderStageFlags stageFlags, uint32_t binding);
	CONCERTO_PUBLIC_API VkWriteDescriptorSet WriteDescriptorBuffer(VkDescriptorType type, VkDescriptorSet dstSet, VkDescriptorBufferInfo* bufferInfo, uint32_t binding);
	CONCERTO_PUBLIC_API VkCommandBufferBeginInfo CommandBufferBeginInfo(VkCommandBufferUsageFlags flags = 0);
	CONCERTO_PUBLIC_API VkSamplerCreateInfo SamplerCreateInfo(VkFilter filter, VkSamplerAddressMode samplerAddressMode = VK_SAMPLER_ADDRESS_MODE_REPEAT);

	CONCERTO_PUBLIC_API VkWriteDescriptorSet WriteDescriptorImage(VkDescriptorType type, VkDescriptorSet pT, VkDescriptorImageInfo* pInfo, Concerto::UInt32 i);
};


#endif //ENGINE_VULKANINITIALIZER_HPP
