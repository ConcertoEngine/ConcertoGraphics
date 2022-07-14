//
// Created by arthur on 30/06/2022.
//

#ifndef CONCERTOGRAPHICS_PIPELINELAYOUT_HPP
#define CONCERTOGRAPHICS_PIPELINELAYOUT_HPP

#include <cstddef>
#include <vector>
#include "vulkan/vulkan.h"
#include "DescriptorSetLayout.hpp"
namespace Concerto::Graphics::Wrapper
{
	class PipelineLayout
	{
	public:
		PipelineLayout(VkDevice device, std::size_t size, const std::vector<DescriptorSetLayout>& descriptorSetLayouts);
		PipelineLayout(PipelineLayout&&) = default;
		PipelineLayout(const PipelineLayout&) = delete;
		PipelineLayout& operator=(PipelineLayout&&) = default;
		PipelineLayout& operator=(const PipelineLayout&) = delete;
		~PipelineLayout();
		VkPipelineLayout get() const;
	private:
		VkDevice _device;
		VkPipelineLayout _pipelineLayout{};
	};

	template<typename T>
	PipelineLayout makePipelineLayout(VkDevice device, const std::vector<DescriptorSetLayout>& descriptorSetLayouts)
	{
		return {device, sizeof(T), descriptorSetLayouts};
	}
} // Concerto::Graphics::Wrapper

#endif //CONCERTOGRAPHICS_PIPELINELAYOUT_HPP
