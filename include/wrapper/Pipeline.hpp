//
// Created by arthur on 10/06/22.
//

#ifndef CONCERTOGRAPHICS_PIPELINE_HPP
#define CONCERTOGRAPHICS_PIPELINE_HPP

#include <unordered_map>
#include <string>
#include "vulkan/vulkan.h"
#include "wrapper/Object.hpp"
#include "Viewport.hpp"
#include "PipelineInfo.hpp"
#include "PipelineLayout.hpp"
#include "DescriptorSet.hpp"

namespace Concerto::Graphics::Wrapper
{

	class Device;

	class Pipeline : public Object<VkPipeline>
	{
	public:
		explicit Pipeline(Device& device, PipelineInfo pipeLineInfo);

		Pipeline(Pipeline&&) = default;

		Pipeline(const Pipeline&) = delete;

		Pipeline& operator=(Pipeline&&) = default;

		Pipeline& operator=(const Pipeline&) = delete;

		VkPipeline BuildPipeline(VkRenderPass renderPass);

		[[nodiscard]] VkPipelineViewportStateCreateInfo BuildViewportState() const;

		[[nodiscard]] VkPipelineColorBlendStateCreateInfo BuildColorBlendState() const;

		struct CreateInfo
		{
			VkPipelineViewportStateCreateInfo viewportState;
			VkPipelineColorBlendStateCreateInfo colorBlend;
			VkGraphicsPipelineCreateInfo pipelineCreateInfo;
		};
	private:
		PipelineInfo _pipelineInfo;
		CreateInfo _createInfo;
	};

} // Concerto

#endif //CONCERTOGRAPHICS_PIPELINE_HPP
