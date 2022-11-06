//
// Created by arthur on 25/10/2022.
//

#ifndef CONCERTOGRAPHICS_INSTANCE_HPP
#define CONCERTOGRAPHICS_INSTANCE_HPP

#include <optional>
#include <span>
#include <string>
#include <vector>
#include "vulkan/vulkan.h"
#include "Version.hpp"

namespace Concerto::Graphics::Wrapper
{
	class PhysicalDevice;

	class Instance
	{
	public:
		Instance(const std::string& appName, const std::string& engineName, const Version& apiVersion,
				const Version& appVersion, const Version& engineVersion, std::span<const char*> extensions,
				std::span<const char*> layers);

		Instance(const Instance&) = delete;

		Instance(Instance&&) = delete;

		Instance& operator=(const Instance&) = delete;

		Instance& operator=(Instance&&) = delete;

		virtual ~Instance() = default;

		[[nodiscard]] Version GetApiVersion() const;

		[[nodiscard]] VkInstance* Get();

		[[nodiscard]] std::span<PhysicalDevice> EnumeratePhysicalDevices() const;

	private:
		VkInstance _instance;
		Version _apiVersion;
		mutable VkResult _lastResult;
		mutable std::optional<std::vector<PhysicalDevice>> _physicalDevices;
	};
} // Concerto::Graphics::Wrapper

#endif //CONCERTOGRAPHICS_INSTANCE_HPP
