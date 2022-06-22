//
// Created by arthur on 10/06/22.
//

#ifndef CONCERTOGRAPHICS_PIPELINEINFO_HPP
#define CONCERTOGRAPHICS_PIPELINEINFO_HPP
#include <vector>
#include "vulkan/vulkan.h"

namespace Concerto::Graphics::Wrapper
{

	struct PipeLineInfo
	{
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
		Viewport viewport;
	};

} // Concerto::Graphics::Wrapper

#endif //CONCERTOGRAPHICS_PIPELINEINFO_HPP