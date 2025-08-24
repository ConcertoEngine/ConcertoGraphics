//
// Created by arthur on 18/08/22.
//

#include "Concerto/Graphics/Backend/Vulkan/Wrapper/PipelineInfo.hpp"

#include "Concerto/Graphics/Backend/Vulkan/Vertex.hpp"
#include "Concerto/Graphics/Backend/Vulkan/Wrapper/VulkanInitializer.hpp"

namespace cct::gfx::vk
{
	PipelineInfo::PipelineInfo(std::vector<VkPipelineShaderStageCreateInfo> shaderStages, VkExtent2D windowExtent, std::shared_ptr<PipelineLayout>& pipelineLayout)
	{
		static VertexInputDescription vertexDescription = GetVertexDescription(); //TODO Remove static
		m_shaderStages = std::move(shaderStages);
		m_vertexInputInfo = VulkanInitializer::VertexInputStateCreateInfo();
		m_vertexInputInfo.pVertexAttributeDescriptions = vertexDescription.attributes.data();
		m_vertexInputInfo.vertexAttributeDescriptionCount = static_cast<UInt32>(vertexDescription.attributes.size());
		m_vertexInputInfo.pVertexBindingDescriptions = vertexDescription.bindings.data();
		m_vertexInputInfo.vertexBindingDescriptionCount = static_cast<UInt32>(vertexDescription.bindings.size());
		m_inputAssembly = VulkanInitializer::InputAssemblyCreateInfo(VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST);
		m_viewport.x = 0.0f;
		m_viewport.y = 0.0f;
		m_viewport.width = static_cast<float>(windowExtent.width);
		m_viewport.height = static_cast<float>(windowExtent.height);
		m_viewport.minDepth = 0.0f;
		m_viewport.maxDepth = 1.0f;
		m_scissor.offset = { 0, 0 };
		m_scissor.extent = windowExtent;
		m_rasterizer = VulkanInitializer::RasterizationStateCreateInfo(VK_POLYGON_MODE_FILL);
		m_multisampling = VulkanInitializer::MultisamplingStateCreateInfo();
		m_colorBlendAttachment = VulkanInitializer::ColorBlendAttachmentState();
		m_pipelineLayout = pipelineLayout;
		m_depthStencil = VulkanInitializer::DepthStencilCreateInfo(true, true, VK_COMPARE_OP_LESS_OR_EQUAL);
	}
} // cct::gfx::vk