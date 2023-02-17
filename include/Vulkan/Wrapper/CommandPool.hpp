//
// Created by arthur on 14/06/22.
//

#ifndef CONCERTOGRAPHICS_COMMANDPOOL_HPP
#define CONCERTOGRAPHICS_COMMANDPOOL_HPP

#include <cstdint>
#include "vulkan/vulkan.h"
#include "Object.hpp"

namespace Concerto::Graphics::Wrapper
{
	class Device;

	/**
	 * @class CommandPool
	 * @brief Represents a VkCommandPool object.
	 *
	 *  CommandPool class is a Wrapper for VkCommandPool, it encapsulate the VkCommandPool object,
	 *  it also provide some utility functions for creating the VkCommandPool.
	 *  A VkCommandPool is used to allocate command buffers from.
	 *
	 * @note CommandPool objects are move-only and cannot be copied.
	 *
	 * @author [Your name]
	 */
	class CommandPool : public Object<VkCommandPool>
	{
	public:
		/**
		* @brief Constructs a new CommandPool object.
		*
		* @param device The Device object used to create the command pool.
		* @param queueFamily The queue family index used to create the command pool.
		*/
		CommandPool(Device& device, std::uint32_t queueFamily);

		CommandPool(CommandPool&&) = default;

		CommandPool(const CommandPool&) = delete;

		CommandPool& operator=(CommandPool&&) = default;

		CommandPool& operator=(const CommandPool&) = delete;

		/**
	    * @brief Resetting a command pool recycles all of the resources from all of the command buffers allocated from
	    * 		 the command pool back to the command pool.
	    */
		void Reset();

	private:
		std::uint32_t _queueFamily;
	};
}

#endif //CONCERTOGRAPHICS_COMMANDPOOL_HPP
