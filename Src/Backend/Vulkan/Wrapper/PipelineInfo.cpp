//
// Created by arthur on 18/08/22.
//

#include "Concerto/Graphics/Backend/Vulkan/Wrapper/PipelineInfo.hpp"

#include "Concerto/Graphics/Backend/Vulkan/Vertex.hpp"
#include "Concerto/Graphics/Backend/Vulkan/Wrapper/VulkanInitializer.hpp"

namespace Concerto::Graphics::Vk
{
	PipelineInfo::PipelineInfo(std::vector<VkPipelineShaderStageCreateInfo> shaderStages, VkExtent2D windowExtent, std::shared_ptr<PipelineLayout>& pipelineLayout)
	{
		static VertexInputDescription vertexDescription = GetVertexDescription(); //TODO Remove static
		_shaderStages = std::move(shaderStages);
		_vertexInputInfo = VulkanInitializer::VertexInputStateCreateInfo();
		_vertexInputInfo.pVertexAttributeDescriptions = vertexDescription.attributes.data();
		_vertexInputInfo.vertexAttributeDescriptionCount = vertexDescription.attributes.size();
		_vertexInputInfo.pVertexBindingDescriptions = vertexDescription.bindings.data();
		_vertexInputInfo.vertexBindingDescriptionCount = vertexDescription.bindings.size();
		_inputAssembly = VulkanInitializer::InputAssemblyCreateInfo(VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST);
		_viewport.x = 0.0f;
		_viewport.y = 0.0f;
		_viewport.width = static_cast<float>(windowExtent.width);
		_viewport.height = static_cast<float>(windowExtent.height);
		_viewport.minDepth = 0.0f;
		_viewport.maxDepth = 1.0f;
		_scissor.offset = { 0, 0 };
		_scissor.extent = windowExtent;
		_rasterizer = VulkanInitializer::RasterizationStateCreateInfo(VK_POLYGON_MODE_FILL);
		_multisampling = VulkanInitializer::MultisamplingStateCreateInfo();
		_colorBlendAttachment = VulkanInitializer::ColorBlendAttachmentState();
		_pipelineLayout = pipelineLayout;
		_depthStencil = VulkanInitializer::DepthStencilCreateInfo(true, true, VK_COMPARE_OP_LESS_OR_EQUAL);
	}
} // Concerto::Graphics::Vk