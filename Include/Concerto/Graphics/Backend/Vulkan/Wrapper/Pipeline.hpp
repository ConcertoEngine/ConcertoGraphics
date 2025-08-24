//
// Created by arthur on 10/06/22.
//

#ifndef CONCERTO_GRAPHICS_PIPELINE_HPP
#define CONCERTO_GRAPHICS_PIPELINE_HPP

#include <unordered_map>
#include <memory>


#include "Concerto/Graphics/Backend/Vulkan/Defines.hpp"

#include "Concerto/Graphics/Backend/Vulkan/Wrapper/Object.hpp"
#include "Concerto/Graphics/Backend/Vulkan/Wrapper/PipelineInfo.hpp"

namespace cct::gfx::vk
{
	class Device;
	class PipelineLayout;

	class CONCERTO_GRAPHICS_VULKAN_BACKEND_API Pipeline : public Object<VkPipeline>
	{
	public:
		Pipeline(Device& device, PipelineInfo pipeLineInfo);

		~Pipeline() override;

		Pipeline(Pipeline&&) = default;
		Pipeline(const Pipeline&) = delete;

		Pipeline& operator=(Pipeline&&) = default;
		Pipeline& operator=(const Pipeline&) = delete;

		[[nodiscard]] VkPipeline BuildPipeline(VkRenderPass renderPass);
		[[nodiscard]] VkPipelineViewportStateCreateInfo BuildViewportState() const;
		[[nodiscard]] VkPipelineColorBlendStateCreateInfo BuildColorBlendState() const;

		[[nodiscard]] std::shared_ptr<PipelineLayout> GetPipelineLayout() const;
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
