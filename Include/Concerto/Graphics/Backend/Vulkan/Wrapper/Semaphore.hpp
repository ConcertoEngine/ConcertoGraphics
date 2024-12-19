//
// Created by arthur on 15/06/22.
//

#ifndef CONCERTO_GRAPHICS_SEMAPHORE_HPP
#define CONCERTO_GRAPHICS_SEMAPHORE_HPP

#include "Concerto/Graphics/Backend/Vulkan/Defines.hpp"
#include "Concerto/Graphics/Backend/Vulkan/Wrapper/Object.hpp"

namespace cct::gfx::vk
{
	class Device;

	/**
	 * @class Semaphore
	 * @brief A class representing a semaphore.
	 *
	 * A semaphore is a synchronization object that can be used to insert a dependency between command buffers submitted to a queue.
	 */
	class CONCERTO_GRAPHICS_VULKAN_BACKEND_API Semaphore : public Object<VkSemaphore>
	{
	public:
		explicit Semaphore(Device& device);

		~Semaphore();

		Semaphore(Semaphore&&) = default;

		Semaphore(const Semaphore&) = delete;

		Semaphore& operator=(Semaphore&&) = default;

		Semaphore& operator=(const Semaphore&) = delete;
	};
} // namespace cct::gfx::vk

#endif //CONCERTO_GRAPHICS_SEMAPHORE_HPP
