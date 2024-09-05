//
// Created by arthur on 12/05/2024.
//

#ifndef CONCERTO_GRAPHICS_VULKAN_VKRHI_HPP
#define CONCERTO_GRAPHICS_VULKAN_VKRHI_HPP

#include <memory>
#include <vector>

#include "Concerto/Graphics/RHI/APIImpl.hpp"
#include "Concerto/Graphics/Backend/Vulkan/Wrapper/Instance.hpp"

namespace Concerto::Graphics
{
	class VkRHI final : public RHI::APIImpl
	{
	public:
		VkRHI() = default;
		~VkRHI() override = default;

		bool Create(RHI::ValidationLevel validationLevel) override;

		[[nodiscard]] std::span<const RHI::DeviceInfo> EnumerateDevices() override;
		std::unique_ptr<RHI::Device> CreateDevice(std::size_t index) override;
		static constexpr inline RHI::DeviceType FromVulkan(VkPhysicalDeviceType deviceType);

	private:
		std::unique_ptr<Vk::Instance> _instance;
		std::vector<RHI::DeviceInfo> _devicesInfo;
		std::vector<RHI::Device> _devices;
	};
}

#endif //CONCERTO_GRAPHICS_VULKAN_VKRHI_HPP