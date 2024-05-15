//
// Created by arthur on 10/06/22.
//

#ifndef CONCERTO_GRAPHICS_PIPELINE_HPP
#define CONCERTO_GRAPHICS_PIPELINE_HPP

#include <unordered_map>
#include <memory>

#include <vulkan/vulkan.h>
#include "Concerto/Graphics/Defines.hpp"

#include "Concerto/Graphics/Backend/Vulkan/Wrapper/Object.hpp"
#include "Concerto/Graphics/Backend/Vulkan/Wrapper/PipelineInfo.hpp"

namespace Concerto::Graphics::Vk
{
	class Device;
	class PipelineLayout;

	/**
	 * @class Pipeline
	 * @brief Class representing a pipeline in the Vulkan API
	 */
	class CONCERTO_GRAPHICS_API Pipeline : public Object<VkPipeline>
	{
	public:
		/**
		 * @brief Constructor for creating a pipeline
		 * @param device The device that the pipeline will be associated with
		 * @param pipeLineInfo The pipeline information required to build the pipeline
		 */
		Pipeline(Device& device, PipelineInfo pipeLineInfo);

		~Pipeline();

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

		[[nodiscard]] std::shared_ptr<PipelineLayout> GetPipelineLayout() const;
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

	using PipelinePtr = std::shared_ptr<Pipeline>;
} // Concerto

#endif //CONCERTO_GRAPHICS_PIPELINE_HPP
