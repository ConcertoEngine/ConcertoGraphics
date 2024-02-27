//
// Created by arthur on 14/06/22.
//

#ifndef CONCERTO_GRAPHICS_COMMANDPOOL_HPP
#define CONCERTO_GRAPHICS_COMMANDPOOL_HPP

#include <vulkan/vulkan.h>

#include "Concerto/Graphics/Defines.hpp"
#include "Concerto/Graphics/Vulkan/Wrapper/Object.hpp"

namespace Concerto::Graphics
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
	class CONCERTO_GRAPHICS_API CommandPool : public Object<VkCommandPool>
	{
	public:
		/**
		* @brief Constructs a new CommandPool object.
		*
		* @param device The Device object used to create the command pool.
		* @param queueFamily The queue family index used to create the command pool.
		*/
		CommandPool(Device& device, UInt32 queueFamily);

		~CommandPool();

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
		UInt32 _queueFamily;
	};
}

#endif //CONCERTO_GRAPHICS_COMMANDPOOL_HPP
