//
// Created by arthur on 15/06/22.
//

#ifndef CONCERTOGRAPHICS_SEMAPHORE_HPP
#define CONCERTOGRAPHICS_SEMAPHORE_HPP

#include "vulkan/vulkan.hpp"
#include "wrapper/Object.hpp"

namespace Concerto::Graphics::Wrapper
{
	class Device;

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
