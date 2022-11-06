//
// Created by arthur on 30/06/2022.
//

#ifndef CONCERTOGRAPHICS_PIPELINELAYOUT_HPP
#define CONCERTOGRAPHICS_PIPELINELAYOUT_HPP

#include <cstddef>
#include <vector>
#include <functional>
#include "vulkan/vulkan.h"
#include "wrapper/Object.hpp"
#include "wrapper/DescriptorSetLayout.hpp"

namespace Concerto::Graphics::Wrapper
{
	class Device;
	class PipelineLayout : public Object<VkPipelineLayout>
	{
	public:
		PipelineLayout(Device& device, std::size_t size,
				const std::vector<std::reference_wrapper<DescriptorSetLayout>>& descriptorSetLayouts);

		PipelineLayout(PipelineLayout&&) = default;

		PipelineLayout(const PipelineLayout&) = delete;

		PipelineLayout& operator=(PipelineLayout&&) = default;

		PipelineLayout& operator=(const PipelineLayout&) = delete;
	};

	template<typename T>
	PipelineLayout makePipelineLayout(Device& device,
			const std::vector<std::reference_wrapper<DescriptorSetLayout>>& descriptorSetLayouts)
	{
		return { device, sizeof(T), descriptorSetLayouts };
	}
} // Concerto::Graphics::Wrapper

#endif //CONCERTOGRAPHICS_PIPELINELAYOUT_HPP
