//
// Created by arthur on 24/03/2022.
//

#ifndef ENGINE_VULKANINITIALIZER_HPP
#define ENGINE_VULKANINITIALIZER_HPP

#include <vulkan/vulkan.hpp>
#include "Concerto/Graphics/Defines.hpp"

namespace VulkanInitializer
{
	VkPipelineShaderStageCreateInfo PipelineShaderStageCreateInfo(VkShaderStageFlagBits stage, VkShaderModule shaderModule);
	VkPipelineVertexInputStateCreateInfo VertexInputStateCreateInfo();
	VkPipelineInputAssemblyStateCreateInfo InputAssemblyCreateInfo(VkPrimitiveTopology topology);
	VkPipelineRasterizationStateCreateInfo RasterizationStateCreateInfo(VkPolygonMode polygonMode);
	VkPipelineMultisampleStateCreateInfo MultisamplingStateCreateInfo();
	VkPipelineColorBlendAttachmentState ColorBlendAttachmentState();
	VkPipelineLayoutCreateInfo PipelineLayoutCreateInfo();
	VkRenderPassBeginInfo RenderPassBeginInfo(VkRenderPass pT, VkExtent2D extent2D, VkFramebuffer framebuffer);
	VkImageCreateInfo ImageCreateInfo(VkFormat format, VkImageUsageFlags usageFlags, VkExtent3D extent);
	VkImageViewCreateInfo ImageViewCreateInfo(VkFormat format, VkImage image, VkImageAspectFlags aspectFlags);
	VkPipelineDepthStencilStateCreateInfo DepthStencilCreateInfo(bool bDepthTest, bool bDepthWrite, VkCompareOp compareOp);
	VkCommandBufferBeginInfo CommandBufferBeginInfo(VkCommandBufferUsageFlagBits bits);
	VkSubmitInfo SubmitInfo(VkCommandBuffer* pT);
	VkPresentInfoKHR PresentInfo();
	VkCommandPoolCreateInfo CommandPoolCreateInfo(uint32_t queueFamilyIndex, VkCommandPoolCreateFlags flags);
	VkFenceCreateInfo FenceCreateInfo(VkFenceCreateFlagBits bits = static_cast<VkFenceCreateFlagBits>(0));
	VkSemaphoreCreateInfo SemaphoreCreateInfo(VkSemaphoreCreateFlags flags = 0);
	VkCommandBufferAllocateInfo CommandBufferAllocateInfo(VkCommandPool pool, int count, VkCommandBufferLevel level = VK_COMMAND_BUFFER_LEVEL_PRIMARY);
	VkFramebufferCreateInfo FramebufferCreateInfo(VkRenderPass pT, VkExtent2D extent2D);
	VkDescriptorSetLayoutBinding DescriptorSetLayoutBinding(VkDescriptorType type, VkShaderStageFlags stageFlags, uint32_t binding);
	VkWriteDescriptorSet WriteDescriptorBuffer(VkDescriptorType type, VkDescriptorSet dstSet, VkDescriptorBufferInfo* bufferInfo , uint32_t binding);
	VkCommandBufferBeginInfo CommandBufferBeginInfo(VkCommandBufferUsageFlags flags = 0);
	VkSamplerCreateInfo SamplerCreateInfo(VkFilter filter, VkSamplerAddressMode samplerAddressMode = VK_SAMPLER_ADDRESS_MODE_REPEAT);

	VkWriteDescriptorSet WriteDescriptorImage(VkDescriptorType type, VkDescriptorSet pT, VkDescriptorImageInfo* pInfo, Concerto::UInt32 i);
};


#endif //ENGINE_VULKANINITIALIZER_HPP
