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
	struct Material
	{
		Material(VkPipelineLayout pipelineLayout) : _pipelineLayout(pipelineLayout) {}
		DescriptorSet _descriptorSet;
		VkPipelineLayout _pipelineLayout;
	};

	class Pipeline
	{
	public:
		explicit Pipeline(VkDevice& device, const PipelineInfo& pipeLineInfo);

		Pipeline(Pipeline&&) = default;

		Pipeline(const Pipeline&) = delete;

		Pipeline& operator=(Pipeline&&) = delete;

		Pipeline& operator=(const Pipeline&) = delete;

		~Pipeline();

		VkPipeline get() const;
		VkPipeline buildPipeline(VkRenderPass renderPass);

		VkPipelineViewportStateCreateInfo buildViewportState() const;

		VkPipelineColorBlendStateCreateInfo buildColorBlendState() const;

		void createMaterial(const std::string& name, VkPipelineLayout pipelineLayout);

		struct CreateInfo
		{
			VkPipelineViewportStateCreateInfo viewportState;
			VkPipelineColorBlendStateCreateInfo colorBlend;
			VkGraphicsPipelineCreateInfo pipelineCreateInfo;
		};
	private:
		VkDevice& _device;
		VkPipeline _pipeline;
		PipelineInfo _pipelineInfo;
		CreateInfo _createInfo;
		std::unordered_map<std::string, Material> _materials;
	};

} // Concerto

#endif //CONCERTOGRAPHICS_PIPELINE_HPP
