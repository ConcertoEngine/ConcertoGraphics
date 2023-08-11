//
// Created by arthur on 15/06/22.
//

#ifndef CONCERTOGRAPHICS_SEMAPHORE_HPP
#define CONCERTOGRAPHICS_SEMAPHORE_HPP

#include <vulkan/vulkan.hpp>
#include <Concerto/Core/Types.hpp>

#include "Concerto/Graphics/Vulkan/Wrapper/Object.hpp"

namespace Concerto::Graphics
{
	class Device;

	/**
	 * @class Semaphore
	 * @brief A class representing a semaphore.
	 *
	 * A semaphore is a synchronization object that can be used to insert a dependency between command buffers submitted to a queue.
	 */
	class CONCERTO_PUBLIC_API Semaphore : public Object<VkSemaphore>
	{
	public:
		explicit Semaphore(Device& device);

		Semaphore(Semaphore&&) = default;

		Semaphore(const Semaphore&) = delete;

		Semaphore& operator=(Semaphore&&) = default;

		Semaphore& operator=(const Semaphore&) = delete;
	};
} // namespace Concerto::Graphics

#endif //CONCERTOGRAPHICS_SEMAPHORE_HPP
