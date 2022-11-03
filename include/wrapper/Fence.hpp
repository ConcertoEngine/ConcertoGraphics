//
// Created by arthur on 15/06/22.
//

#ifndef CONCERTOGRAPHICS_FENCE_HPP
#define CONCERTOGRAPHICS_FENCE_HPP

#include "vulkan/vulkan.hpp"
#include "wrapper/Object.hpp"

namespace Concerto::Graphics::Wrapper
{
	class Device;

	class Fence : public Object<VkFence>
	{
	public:
		explicit Fence(Device& device, bool signaled = true);

		Fence(Fence&&) = default;

		Fence(const Fence&) = delete;

		Fence& operator=(Fence&&) = default;

		Fence& operator=(const Fence&) = delete;

		~Fence();

		void wait(std::uint64_t timeout);

		void reset();
	};
} // namespace Concerto::Graphics::Wrapper

#endif //CONCERTOGRAPHICS_FENCE_HPP
