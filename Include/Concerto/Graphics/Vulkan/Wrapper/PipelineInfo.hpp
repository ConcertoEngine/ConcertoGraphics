//
// Created by arthur on 10/06/22.
//

#ifndef CONCERTO_GRAPHICS_PIPELINEINFO_HPP
#define CONCERTO_GRAPHICS_PIPELINEINFO_HPP

#include <vector>
#include <memory>

#include <vulkan/vulkan.h>

#include "Concerto/Graphics/Vulkan/Wrapper/PipelineLayout.hpp"

namespace Concerto::Graphics
{

	class CONCERTO_GRAPHICS_API PipelineInfo
	{
	public:
		PipelineInfo(std::vector<VkPipelineShaderStageCreateInfo> shaderStages, VkExtent2D windowExtent, std::shared_ptr<PipelineLayout>& pipelineLayout);

		std::vector<VkPipelineShaderStageCreateInfo> _shaderStages;
		VkPipelineVertexInputStateCreateInfo _vertexInputInfo;
		VkPipelineInputAssemblyStateCreateInfo _inputAssembly;
		VkViewport _viewport;
		VkRect2D _scissor;
		VkPipelineRasterizationStateCreateInfo _rasterizer;
		VkPipelineColorBlendAttachmentState _colorBlendAttachment;
		VkPipelineMultisampleStateCreateInfo _multisampling;
		std::shared_ptr<PipelineLayout> _pipelineLayout;
		VkPipelineDepthStencilStateCreateInfo _depthStencil;
	};

} // Concerto::Graphics::Wrapper

#endif //CONCERTO_GRAPHICS_PIPELINEINFO_HPP