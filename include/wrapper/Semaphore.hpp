//
// Created by arthur on 15/06/22.
//

#ifndef CONCERTOGRAPHICS_SEMAPHORE_HPP
#define CONCERTOGRAPHICS_SEMAPHORE_HPP

#include "vulkan/vulkan.hpp"

namespace Concerto::Graphics::Wrapper
{
	class Semaphore
	{
	public:
		explicit Semaphore(VkDevice device);

		Semaphore(Semaphore&&) = default;

		Semaphore(const Semaphore&) = delete;

		Semaphore& operator=(Semaphore&&) = default;

		Semaphore& operator=(const Semaphore&) = delete;

		~Semaphore();

		VkSemaphore Get() const;
	private:
		VkDevice _device;
		VkSemaphore _semaphore;
	};
} // namespace Concerto::Graphics::Wrapper

#endif //CONCERTOGRAPHICS_SEMAPHORE_HPP
