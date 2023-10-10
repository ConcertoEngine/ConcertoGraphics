//
// Created by arthur on 30/06/2022.
//

#ifndef CONCERTOGRAPHICS_PIPELINELAYOUT_HPP
#define CONCERTOGRAPHICS_PIPELINELAYOUT_HPP

#include <cstddef>
#include <vector>
#include <functional>

#include <vulkan/vulkan.h>
#include "Concerto/Graphics/Defines.hpp"

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
	class CONCERTO_GRAPHICS_API PipelineLayout : public Object<VkPipelineLayout>
	{
	public:
		PipelineLayout(Device& device, std::vector<DescriptorSetLayoutPtr> descriptorSetLayouts);

		PipelineLayout(PipelineLayout&&) = default;

		PipelineLayout(const PipelineLayout&) = delete;

		PipelineLayout& operator=(PipelineLayout&&) = default;

		PipelineLayout& operator=(const PipelineLayout&) = delete;

		const std::vector<DescriptorSetLayoutPtr>& GetDescriptorSetLayouts() const;
	private:
		std::vector<DescriptorSetLayoutPtr> _descriptorSetLayouts;
	};
} // Concerto::Graphics::Wrapper

#endif //CONCERTOGRAPHICS_PIPELINELAYOUT_HPP
