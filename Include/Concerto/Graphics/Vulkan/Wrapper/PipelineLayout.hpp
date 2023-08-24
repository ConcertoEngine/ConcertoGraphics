//
// Created by arthur on 30/06/2022.
//

#ifndef CONCERTOGRAPHICS_PIPELINELAYOUT_HPP
#define CONCERTOGRAPHICS_PIPELINELAYOUT_HPP

#include <cstddef>
#include <vector>
#include <functional>

#include <vulkan/vulkan.h>
#include <Concerto/Core/Types.hpp>

#include "Concerto/Graphics/Vulkan/Wrapper/Object.hpp"
#include "Concerto/Graphics/Vulkan/Wrapper/DescriptorSetLayout.hpp"

namespace Concerto::Graphics
{
	class Device;

	/**
	 * @class Pipeline
	 *
	 * @brief A Wrapper class for creating and managing VkPipelineLayout.
	 * The pipeline layout represents a sequence of descriptor sets with each having a specific layout. This sequence of layouts is used to determine the interface between shader stages and shader resources
	 */
	class CONCERTO_PUBLIC_API PipelineLayout : public Object<VkPipelineLayout>
	{
	public:
		PipelineLayout(Device& device, const std::vector<VkDescriptorSetLayout>& descriptorSetLayouts);

		PipelineLayout(PipelineLayout&&) = default;

		PipelineLayout(const PipelineLayout&) = delete;

		PipelineLayout& operator=(PipelineLayout&&) = default;

		PipelineLayout& operator=(const PipelineLayout&) = delete;
	};
} // Concerto::Graphics::Wrapper

#endif //CONCERTOGRAPHICS_PIPELINELAYOUT_HPP
