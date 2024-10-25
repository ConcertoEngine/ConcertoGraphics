//
// Created by arthur on 30/06/2022.
//

#ifndef CONCERTO_GRAPHICS_PIPELINELAYOUT_HPP
#define CONCERTO_GRAPHICS_PIPELINELAYOUT_HPP

#include <vector>
#include <memory>



#include "Concerto/Graphics/Backend/Vulkan/Defines.hpp"
#include "Concerto/Graphics/Backend/Vulkan/Wrapper/Object.hpp"

namespace Concerto::Graphics::Vk
{
	class Device;
	class DescriptorSetLayout;

	/**
	 * @class Pipeline
	 *
	 * @brief A Wrapper class for creating and managing VkPipelineLayout.
	 * The pipeline layout represents a sequence of descriptor sets with each having a specific layout. This sequence of layouts is used to determine the interface between shader stages and shader resources
	 */
	class CONCERTO_GRAPHICS_VULKAN_BACKEND_API PipelineLayout : public Object<VkPipelineLayout>
	{
	public:
		PipelineLayout(Device& device, std::vector<std::shared_ptr<DescriptorSetLayout>> descriptorSetLayouts);

		~PipelineLayout();

		PipelineLayout(PipelineLayout&&) = default;

		PipelineLayout(const PipelineLayout&) = delete;

		PipelineLayout& operator=(PipelineLayout&&) = default;

		PipelineLayout& operator=(const PipelineLayout&) = delete;

		[[nodiscard]] const std::vector<std::shared_ptr<DescriptorSetLayout>>& GetDescriptorSetLayouts() const;
	private:
		std::vector<std::shared_ptr<DescriptorSetLayout>> _descriptorSetLayouts;
	};
} // Concerto::Graphics::Vk

#endif //CONCERTO_GRAPHICS_PIPELINELAYOUT_HPP
