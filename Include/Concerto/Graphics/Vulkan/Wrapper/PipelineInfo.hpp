//
// Created by arthur on 10/06/22.
//

#ifndef CONCERTOGRAPHICS_PIPELINEINFO_HPP
#define CONCERTOGRAPHICS_PIPELINEINFO_HPP

#include <utility>
#include <vector>

#include <vulkan/vulkan.h>

#include "Concerto/Graphics/Vulkan/Wrapper/PipelineLayout.hpp"
#include "Concerto/Graphics/Vulkan/Vertex.hpp"

namespace Concerto::Graphics
{

	class PipelineInfo
	{
	public:
		PipelineInfo(std::vector<VkPipelineShaderStageCreateInfo> shaderStages, VkExtent2D windowExtent, PipelineLayout& pipelineLayout);

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