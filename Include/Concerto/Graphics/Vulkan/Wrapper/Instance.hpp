//
// Created by arthur on 25/10/2022.
//

#ifndef CONCERTOGRAPHICS_INSTANCE_HPP
#define CONCERTOGRAPHICS_INSTANCE_HPP

#include <optional>
#include <span>
#include <string>
#include <vector>

#include <vulkan/vulkan.h>
#include "Concerto/Graphics/Defines.hpp"

#include "Concerto/Graphics/Version.hpp"
#include "Concerto/Graphics/Vulkan/Wrapper/PhysicalDevice.hpp"

namespace Concerto::Graphics
{
	/**
	* @class Instance
	*
	* @brief Represents a Vulkan instance.
	*
	* The Instance class represents a Vulkan instance, which is the root object of the Vulkan API.
	* It is responsible for creating and managing other objects in the Vulkan API, such as physical devices and logical devices.
	*/
	class CONCERTO_GRAPHICS_API Instance
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

		Instance(Instance&&) = delete;

		Instance& operator=(const Instance&) = delete;

		Instance& operator=(Instance&&) = delete;

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

	private:
		VkInstance _instance;
		Version _apiVersion;
		mutable VkResult _lastResult;
		mutable std::optional<std::vector<PhysicalDevice>> _physicalDevices;
	};
} // Concerto::Graphics::Wrapper

#endif //CONCERTOGRAPHICS_INSTANCE_HPP
