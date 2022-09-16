//
// Created by arthur on 10/06/22.
//

#ifndef CONCERTOGRAPHICS_PIPELINE_HPP
#define CONCERTOGRAPHICS_PIPELINE_HPP

#include <unordered_map>
#include <string>
#include "vulkan/vulkan.h"
#include "Viewport.hpp"
#include "PipelineInfo.hpp"
#include "PipelineLayout.hpp"
#include "DescriptorSet.hpp"

namespace Concerto::Graphics::Wrapper
{


	class Pipeline
	{
	public:
		explicit Pipeline(VkDevice& device, PipelineInfo  pipeLineInfo);

		Pipeline(Pipeline&&) = default;

		Pipeline(const Pipeline&) = delete;

		Pipeline& operator=(Pipeline&&) = delete;

		Pipeline& operator=(const Pipeline&) = delete;

		~Pipeline();

		[[nodiscard]] VkPipeline Get() const;

		VkPipeline buildPipeline(VkRenderPass renderPass);

		[[nodiscard]] VkPipelineViewportStateCreateInfo buildViewportState() const;

		[[nodiscard]] VkPipelineColorBlendStateCreateInfo buildColorBlendState() const;

		struct CreateInfo
		{
			VkPipelineViewportStateCreateInfo viewportState;
			VkPipelineColorBlendStateCreateInfo colorBlend;
			VkGraphicsPipelineCreateInfo pipelineCreateInfo;
		};
	private:
		VkDevice& _device;
		VkPipeline _pipeline{};
		PipelineInfo _pipelineInfo;
		CreateInfo _createInfo;
	};

} // Concerto

#endif //CONCERTOGRAPHICS_PIPELINE_HPP
