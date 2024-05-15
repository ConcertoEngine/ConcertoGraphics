//
// Created by arthur on 24/03/2022.
//

#ifndef ENGINE_VULKANINITIALIZER_HPP
#define ENGINE_VULKANINITIALIZER_HPP

#include <vulkan/vulkan.hpp>
#include "Concerto/Graphics/Defines.hpp"

namespace VulkanInitializer
{
	CONCERTO_GRAPHICS_API VkPipelineShaderStageCreateInfo PipelineShaderStageCreateInfo(VkShaderStageFlagBits stage, VkShaderModule shaderModule);
	CONCERTO_GRAPHICS_API VkPipelineVertexInputStateCreateInfo VertexInputStateCreateInfo();
	CONCERTO_GRAPHICS_API VkPipelineInputAssemblyStateCreateInfo InputAssemblyCreateInfo(VkPrimitiveTopology topology);
	CONCERTO_GRAPHICS_API VkPipelineRasterizationStateCreateInfo RasterizationStateCreateInfo(VkPolygonMode polygonMode);
	CONCERTO_GRAPHICS_API VkPipelineMultisampleStateCreateInfo MultisamplingStateCreateInfo();
	CONCERTO_GRAPHICS_API VkPipelineColorBlendAttachmentState ColorBlendAttachmentState();
	CONCERTO_GRAPHICS_API VkPipelineLayoutCreateInfo PipelineLayoutCreateInfo();
	CONCERTO_GRAPHICS_API VkRenderPassBeginInfo RenderPassBeginInfo(VkRenderPass pT, VkExtent2D extent2D, VkFramebuffer framebuffer);
	CONCERTO_GRAPHICS_API VkImageCreateInfo ImageCreateInfo(VkFormat format, VkImageUsageFlags usageFlags, VkExtent3D extent);
	CONCERTO_GRAPHICS_API VkImageViewCreateInfo ImageViewCreateInfo(VkFormat format, VkImage image, VkImageAspectFlags aspectFlags);
	CONCERTO_GRAPHICS_API VkPipelineDepthStencilStateCreateInfo DepthStencilCreateInfo(bool bDepthTest, bool bDepthWrite, VkCompareOp compareOp);
	CONCERTO_GRAPHICS_API VkCommandBufferBeginInfo CommandBufferBeginInfo(VkCommandBufferUsageFlagBits bits);
	CONCERTO_GRAPHICS_API VkSubmitInfo SubmitInfo(VkCommandBuffer* pT);
	CONCERTO_GRAPHICS_API VkPresentInfoKHR PresentInfo();
	CONCERTO_GRAPHICS_API VkCommandPoolCreateInfo CommandPoolCreateInfo(uint32_t queueFamilyIndex, VkCommandPoolCreateFlags flags);
	CONCERTO_GRAPHICS_API VkFenceCreateInfo FenceCreateInfo(VkFenceCreateFlagBits bits = static_cast<VkFenceCreateFlagBits>(0));
	CONCERTO_GRAPHICS_API VkSemaphoreCreateInfo SemaphoreCreateInfo(VkSemaphoreCreateFlags flags = 0);
	CONCERTO_GRAPHICS_API VkCommandBufferAllocateInfo CommandBufferAllocateInfo(VkCommandPool pool, int count, VkCommandBufferLevel level = VK_COMMAND_BUFFER_LEVEL_PRIMARY);
	CONCERTO_GRAPHICS_API VkFramebufferCreateInfo FramebufferCreateInfo(VkRenderPass pT, VkExtent2D extent2D);
	CONCERTO_GRAPHICS_API VkDescriptorSetLayoutBinding DescriptorSetLayoutBinding(VkDescriptorType type, VkShaderStageFlags stageFlags, uint32_t binding);
	CONCERTO_GRAPHICS_API VkWriteDescriptorSet WriteDescriptorBuffer(VkDescriptorType type, VkDescriptorSet dstSet, VkDescriptorBufferInfo* bufferInfo, uint32_t binding);
	CONCERTO_GRAPHICS_API VkCommandBufferBeginInfo CommandBufferBeginInfo(VkCommandBufferUsageFlags flags = 0);
	CONCERTO_GRAPHICS_API VkSamplerCreateInfo SamplerCreateInfo(VkFilter filter, VkSamplerAddressMode samplerAddressMode = VK_SAMPLER_ADDRESS_MODE_REPEAT);

	CONCERTO_GRAPHICS_API VkWriteDescriptorSet WriteDescriptorImage(VkDescriptorType type, VkDescriptorSet pT, VkDescriptorImageInfo* pInfo, Concerto::UInt32 i);
};


#endif //ENGINE_VULKANINITIALIZER_HPP
