//
// Created by arthur on 15/06/22.
//

#ifndef CONCERTOGRAPHICS_FENCE_HPP
#define CONCERTOGRAPHICS_FENCE_HPP

#include "vulkan/vulkan.hpp"

namespace Concerto::Graphics::Wrapper
{
	class Fence
	{
	public:
		Fence(VkDevice device, bool signaled = true);

		Fence(Fence&&) = default;

		Fence(const Fence&) = delete;

		Fence& operator=(Fence&&) = default;

		Fence& operator=(const Fence&) = delete;

		~Fence();
		VkFence Get() const;
		void wait(std::uint64_t timeout);
		void reset();


	private:
		VkDevice _device;
		VkFence _fence{};
	};
} // namespace Concerto::Graphics::Wrapper

#endif //CONCERTOGRAPHICS_FENCE_HPP
