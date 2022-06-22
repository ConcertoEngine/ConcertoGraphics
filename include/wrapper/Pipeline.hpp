//
// Created by arthur on 10/06/22.
//

#ifndef CONCERTOGRAPHICS_PIPELINE_HPP
#define CONCERTOGRAPHICS_PIPELINE_HPP

#include "vulkan/vulkan.h"
#include "Viewport.hpp"
#include "PipeLineInfo.hpp"

namespace Concerto::Graphics::Wrapper
{
	class Pipeline
	{
	public:
		explicit Pipeline(VkDevice& device, const PipeLineInfo& pipeLineInfo);

		Pipeline(Pipeline&&) = default;

		Pipeline(const Pipeline&) = delete;

		Pipeline& operator=(Pipeline&&) = default;

		Pipeline& operator=(const Pipeline&) = delete;

		~Pipeline() = default;

		VkPipeline buildPipeline(const VkRenderPass& renderPass) const;

		VkPipelineViewportStateCreateInfo buildViewportState() const;

		VkPipelineColorBlendStateCreateInfo buildColorBlendState() const;

		struct CreateInfo
		{
			VkPipelineViewportStateCreateInfo viewportState;
			VkPipelineColorBlendStateCreateInfo colorBlend;
			VkGraphicsPipelineCreateInfo pipelineCreateInfo;
		};
	private:
		VkDevice& _device;
		PipeLineInfo _pipeLineInfo;
		CreateInfo _createInfo;
		PipeLineInfo _pipelineInfo;
	};

} // Concerto

#endif //CONCERTOGRAPHICS_PIPELINE_HPP
