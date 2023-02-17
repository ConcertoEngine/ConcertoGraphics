//
// Created by arthur on 15/06/22.
//

#ifndef CONCERTOGRAPHICS_SEMAPHORE_HPP
#define CONCERTOGRAPHICS_SEMAPHORE_HPP

#include "vulkan/vulkan.hpp"
#include "Object.hpp"

namespace Concerto::Graphics::Wrapper
{
	class Device;

	/**
	 * @class Semaphore
	 * @brief A class representing a semaphore.
	 *
	 * A semaphore is a synchronization object that can be used to insert a dependency between command buffers submitted to a queue.
	 */
	class Semaphore : public Object<VkSemaphore>
	{
	public:
		explicit Semaphore(Device& device);

		Semaphore(Semaphore&&) = default;

		Semaphore(const Semaphore&) = delete;

		Semaphore& operator=(Semaphore&&) = default;

		Semaphore& operator=(const Semaphore&) = delete;
	};
} // namespace Concerto::Graphics::Wrapper

#endif //CONCERTOGRAPHICS_SEMAPHORE_HPP
