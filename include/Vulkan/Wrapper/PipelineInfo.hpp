//
// Created by arthur on 10/06/22.
//

#ifndef CONCERTOGRAPHICS_PIPELINEINFO_HPP
#define CONCERTOGRAPHICS_PIPELINEINFO_HPP

#include <utility>
#include <vector>
#include "vulkan/vulkan.h"
#include "PipelineLayout.hpp"
#include "Vulkan/Vertex.hpp"
#include "VulkanInitializer.hpp"

namespace Concerto::Graphics::Wrapper
{

	struct PipelineInfo
	{
		PipelineInfo() = default;

		PipelineInfo(std::vector<VkPipelineShaderStageCreateInfo> shaderStages, VkExtent2D windowExtent,
				PipelineLayout& pipelineLayout)
		{
			static VertexInputDescription vertexDescription = Vertex::GetVertexDescription(); //TODO Remove static
			_shaderStages = std::move(shaderStages);
			_vertexInputInfo = VulkanInitializer::VertexInputStateCreateInfo();
			_vertexInputInfo.pVertexAttributeDescriptions = vertexDescription.attributes.data();
			_vertexInputInfo.vertexAttributeDescriptionCount = vertexDescription.attributes.size();
			_vertexInputInfo.pVertexBindingDescriptions = vertexDescription.bindings.data();
			_vertexInputInfo.vertexBindingDescriptionCount = vertexDescription.bindings.size();
			_inputAssembly = VulkanInitializer::InputAssemblyCreateInfo(VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST);
			_viewport.x = 0.0f;
			_viewport.y = 0.0f;
			_viewport.width = (float)windowExtent.width;
			_viewport.height = (float)windowExtent.height;
			_viewport.minDepth = 0.0f;
			_viewport.maxDepth = 1.0f;
			_scissor.offset = { 0, 0 };
			_scissor.extent = windowExtent;
			_rasterizer = VulkanInitializer::RasterizationStateCreateInfo(VK_POLYGON_MODE_FILL);
			_multisampling = VulkanInitializer::MultisamplingStateCreateInfo();
			_colorBlendAttachment = VulkanInitializer::ColorBlendAttachmentState();
			_pipelineLayout = *pipelineLayout.Get();
			_depthStencil = VulkanInitializer::DepthStencilCreateInfo(true, true, VK_COMPARE_OP_LESS_OR_EQUAL);
		}

		std::vector<VkPipelineShaderStageCreateInfo> _shaderStages;
		VkPipelineVertexInputStateCreateInfo _vertexInputInfo;
		VkPipelineInputAssemblyStateCreateInfo _inputAssembly;
		VkViewport _viewport;
		VkRect2D _scissor;
		VkPipelineRasterizationStateCreateInfo _rasterizer;
		VkPipelineColorBlendAttachmentState _colorBlendAttachment;
		VkPipelineMultisampleStateCreateInfo _multisampling;
		VkPipelineLayout _pipelineLayout;
		VkPipelineDepthStencilStateCreateInfo _depthStencil;
	};

} // Concerto::Graphics::Wrapper

#endif //CONCERTOGRAPHICS_PIPELINEINFO_HPP