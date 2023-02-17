//
// Created by arthur on 10/06/22.
//

#ifndef CONCERTOGRAPHICS_PIPELINE_HPP
#define CONCERTOGRAPHICS_PIPELINE_HPP

#include <unordered_map>
#include <string>
#include "vulkan/vulkan.h"
#include "Object.hpp"
#include "Viewport.hpp"
#include "PipelineInfo.hpp"
#include "PipelineLayout.hpp"
#include "DescriptorSet.hpp"

namespace Concerto::Graphics::Wrapper
{

	class Device;

	/**
	 * @class Pipeline
	 * @brief Class representing a pipeline in the Vulkan API
	 */
	class Pipeline : public Object<VkPipeline>
	{
	public:
		/**
		 * @brief Constructor for creating a pipeline
		 * @param device The device that the pipeline will be associated with
		 * @param pipeLineInfo The pipeline information required to build the pipeline
		 */
		explicit Pipeline(Device& device, PipelineInfo pipeLineInfo);

		Pipeline(Pipeline&&) = default;

		Pipeline(const Pipeline&) = delete;

		Pipeline& operator=(Pipeline&&) = default;

		Pipeline& operator=(const Pipeline&) = delete;

		/**
		 * @brief Builds the pipeline with the given render pass
		 * @param renderPass The render pass that the pipeline will be associated with
		 * @return The built pipeline
		 */
		VkPipeline BuildPipeline(VkRenderPass renderPass);

		/**
		 * @brief Builds the viewport state for the pipeline
		 * @return The viewport state create info struct
		 */
		[[nodiscard]] VkPipelineViewportStateCreateInfo BuildViewportState() const;

		/**
		 * @brief Builds the color blend state for the pipeline
		 * @return The color blend state create info struct
		 */
		[[nodiscard]] VkPipelineColorBlendStateCreateInfo BuildColorBlendState() const;

		/**
		 * @struct CreateInfo
		 * @brief Struct containing the create info for building the pipeline
		 */
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
