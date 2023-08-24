//
// Created by arthur on 16/06/22.
//

#ifndef CONCERTOGRAPHICS_DESCRIPTORSET_HPP
#define CONCERTOGRAPHICS_DESCRIPTORSET_HPP

#include <memory>

#include <vulkan/vulkan.h>
#include "Concerto/Graphics/Defines.hpp"

#include "Concerto/Graphics/Vulkan/Wrapper/Object.hpp"

namespace Concerto::Graphics
{
	class DescriptorPool;
	class DescriptorSetLayout;
	class Sampler;
	class ImageView;
	class Device;

	/**
	* @class DescriptorSet
	* @brief This class represents a VkDescriptorSet.
	*  A descriptor set is a collection of descriptors that are used to
	*  describe resources to the Shaders in a pipeline.
	*
	* This class is a Wrapper around the VkDescriptorSet handle and provides
	* a simpler interface for interacting with descriptor sets.
	*/
	class CONCERTO_GRAPHICS_API DescriptorSet : public Object<VkDescriptorSet>
	{
	public:
		/**
		* @brief Constructs a new descriptor set from a device, descriptor pool, and descriptor set layout.
		*
		* @param device The device to create the descriptor set on.
		* @param pool The descriptor pool to allocate the descriptor set from.
		* @param descriptorSetLayout The layout of the descriptor set.
		*/
		DescriptorSet(Device& device, DescriptorPool& pool,	DescriptorSetLayout& descriptorSetLayout);

		DescriptorSet(DescriptorSet&&) noexcept;

		DescriptorSet(const DescriptorSet&) = delete;

		DescriptorSet& operator=(DescriptorSet&&) noexcept = default;

		DescriptorSet& operator=(const DescriptorSet&) = delete;

		/**
		* @brief Writes an image sampler descriptor to the descriptor set.
		*
		* @param sampler The sampler to write to the descriptor set.
		* @param imageView The image view to write to the descriptor set.
		* @param imageLayout The layout of the image in the descriptor set.
		*/
		void WriteImageSamplerDescriptor(Sampler& sampler, ImageView& imageView,
				VkImageLayout imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);
	private:
		DescriptorPool* _pool{};
	};
	using DescriptorSetPtr = std::shared_ptr<DescriptorSet>;
}

#endif //CONCERTOGRAPHICS_DESCRIPTORSET_HPP
