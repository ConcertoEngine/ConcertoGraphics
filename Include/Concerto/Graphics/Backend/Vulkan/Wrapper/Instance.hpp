//
// Created by arthur on 25/10/2022.
//

#ifndef CONCERTO_GRAPHICS_INSTANCE_HPP
#define CONCERTO_GRAPHICS_INSTANCE_HPP

#include <optional>
#include <span>
#include <string>
#include <unordered_set>
#include <vector>

#include "Concerto/Graphics/Backend/Vulkan/Defines.hpp"
#include "Concerto/Graphics/Version.hpp"
#include "Concerto/Graphics/Backend/Vulkan/Wrapper/PhysicalDevice.hpp"

namespace cct::gfx::vk
{
	class CONCERTO_GRAPHICS_VULKAN_BACKEND_API Instance
	{
	public:
		Instance(const std::string& appName, const std::string& engineName, const Version& apiVersion,
				const Version& appVersion, const Version& engineVersion, std::span<const char*> extensions,
				std::span<const char*> layers);
		virtual ~Instance() = default;

		Instance(const Instance&) = delete;
		Instance(Instance&&) = default;

		Instance& operator=(const Instance&) = delete;
		Instance& operator=(Instance&&) = default;


		[[nodiscard]] Version GetApiVersion() const;
		[[nodiscard]] VkInstance* Get();
		[[nodiscard]] std::span<PhysicalDevice> EnumeratePhysicalDevices() const;
		[[nodiscard]] inline VkResult GetLastError() const;

		bool IsExtensionEnabled(const std::string& ext) const; //use string_view instead

		#define CONCERTO_VULKAN_BACKEND_INSTANCE_FUNCTION(func) PFN_##func func = nullptr;
		#define CONCERTO_VULKAN_BACKEND_INSTANCE_EXT_FUNCTION(func, ...) CONCERTO_VULKAN_BACKEND_INSTANCE_FUNCTION(func)
		#include "Concerto/Graphics/Backend/Vulkan/Wrapper/InstanceFunction.hpp"

		static PFN_vkGetInstanceProcAddr vkGetInstanceProcAddr;
	private:
		VkInstance m_instance;
		Version m_apiVersion;
		mutable VkResult m_lastResult;
		mutable std::optional<std::vector<PhysicalDevice>> m_physicalDevices;
		std::unordered_set<std::string> m_loadedExtensions;
		std::unordered_set<std::string> m_loadedLayers;
	};
} // cct::gfx::vk

#endif //CONCERTO_GRAPHICS_INSTANCE_HPP
