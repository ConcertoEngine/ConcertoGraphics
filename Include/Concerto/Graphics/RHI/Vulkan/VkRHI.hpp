//
// Created by arthur on 12/05/2024.
//

#ifndef CONCERTO_GRAPHICS_VULKAN_VKRHI_HPP
#define CONCERTO_GRAPHICS_VULKAN_VKRHI_HPP

#include <memory>
#include <vector>

#include "Concerto/Graphics/RHI/APIImpl.hpp"
#include "Concerto/Graphics/Backend/Vulkan/Wrapper/Instance/Instance.hpp"

namespace cct::gfx
{
	class VkRHI final : public rhi::APIImpl
	{
	public:
		VkRHI() = default;
		~VkRHI() override = default;

		bool Create(rhi::ValidationLevel validationLevel) override;

		[[nodiscard]] std::span<const rhi::DeviceInfo> EnumerateDevices() override;
		std::unique_ptr<rhi::Device> CreateDevice(std::size_t index) override;
		static constexpr inline rhi::DeviceType FromVulkan(VkPhysicalDeviceType deviceType);

	private:
		std::unique_ptr<vk::Instance> m_instance;
		std::vector<rhi::DeviceInfo> m_devicesInfo;
		std::vector<rhi::Device> m_devices;
	};
}

#endif //CONCERTO_GRAPHICS_VULKAN_VKRHI_HPP