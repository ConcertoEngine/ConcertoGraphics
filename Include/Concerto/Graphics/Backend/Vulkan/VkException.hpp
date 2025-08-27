//
// Created by arthur on 27/08/25.
//

#ifndef CONCERTO_GRAPHICS_BACKEND_VULKAN_EXCEPTION_HPP
#define CONCERTO_GRAPHICS_BACKEND_VULKAN_EXCEPTION_HPP

#include <stdexcept>
#include <string>

namespace cct::gfx::vk
{
	class VkException : public std::runtime_error
	{
	public:
		VkException(VkResult result)
			: std::runtime_error("Vulkan error occurred"), m_result(result) {}

		VkResult GetResult() const { return m_result; }

	private:
		VkResult m_result;
	};
}

#endif // CONCERTO_GRAPHICS_BACKEND_VULKAN_EXCEPTION_HPP
