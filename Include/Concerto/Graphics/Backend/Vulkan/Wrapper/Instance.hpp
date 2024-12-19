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
	/**
	* @class Instance
	*
	* @brief Represents a Vulkan instance.
	*
	* The Instance class represents a Vulkan instance, which is the root object of the Vulkan API.
	* It is responsible for creating and managing other objects in the Vulkan API, such as physical devices and logical devices.
	*/
	class CONCERTO_GRAPHICS_VULKAN_BACKEND_API Instance
	{
	public:
		/**
		* @brief Constructs a new instance.
		*
		* @param appName The name of the application.
		* @param engineName The name of the engine.
		* @param apiVersion The version of the Vulkan API.
		* @param appVersion The version of the application.
		* @param engineVersion The version of the engine.
		* @param extensions The extensions to enable.
		* @param layers The layers to enable.
		*/
		Instance(const std::string& appName, const std::string& engineName, const Version& apiVersion,
				const Version& appVersion, const Version& engineVersion, std::span<const char*> extensions,
				std::span<const char*> layers);

		Instance(const Instance&) = delete;

		Instance(Instance&&) = default;

		Instance& operator=(const Instance&) = delete;

		Instance& operator=(Instance&&) = default;

		virtual ~Instance() = default;

		/**
		* @brief Gets the API version of the instance.
		*
		* @return The API version of the instance.
		*/
		[[nodiscard]] Version GetApiVersion() const;

		/**
		* @brief Gets the raw vulkan instance handle.
		*
		* @return A pointer to the raw vulkan instance handle.
		*/
		[[nodiscard]] VkInstance* Get();

		/**
	   * @brief Enumerates all physical devices present in the system.
	   *
	   * @return A span of all physical devices present in the system.
	   *
	   * @see PhysicalDevice
	   */
		[[nodiscard]] std::span<PhysicalDevice> EnumeratePhysicalDevices() const;

		[[nodiscard]] inline VkResult GetLastError() const;

		bool IsExtensionEnabled(const std::string& ext) const; //use string_view instead

		#define CONCERTO_VULKAN_BACKEND_INSTANCE_FUNCTION(func) PFN_##func func = nullptr;
		#define CONCERTO_VULKAN_BACKEND_INSTANCE_EXT_FUNCTION(func, ...) CONCERTO_VULKAN_BACKEND_INSTANCE_FUNCTION(func)
		#include "Concerto/Graphics/Backend/Vulkan/Wrapper/InstanceFunction.hpp"

		static PFN_vkGetInstanceProcAddr vkGetInstanceProcAddr;
	private:
		VkInstance _instance;
		Version _apiVersion;
		mutable VkResult _lastResult;
		mutable std::optional<std::vector<PhysicalDevice>> _physicalDevices;
		std::unordered_set<std::string> _loadedExtensions;
		std::unordered_set<std::string> _loadedLayers;
	};
} // cct::gfx::vk

#endif //CONCERTO_GRAPHICS_INSTANCE_HPP
