//
// Created by arthur on 03/11/2022.
//

#ifndef CONCERTO_GRAPHICS_OBJECT_HPP
#define CONCERTO_GRAPHICS_OBJECT_HPP

#include <functional>
#include <vulkan/vulkan.h>

namespace Concerto::Graphics::Vk
{
	class Device;

	template<typename vkType>
	class Object
	{
	public:
		explicit Object(Device& device);

		Object(const Object&) = delete;

		~Object() = default;

		Object(Object&&) noexcept;

		Object& operator=(const Object&) = delete;

		Object& operator=(Object&&) noexcept;

		[[nodiscard]] vkType* Get() const;
		[[nodiscard]] bool IsNull() const;
		[[nodiscard]] Device* GetDevice() const;
		[[nodiscard]] VkResult GetLastResult() const;

	protected:
		vkType _handle;
		Device* _device;
		mutable VkResult _lastResult;
	};
}

#include "Object.inl"

#endif //CONCERTO_GRAPHICS_OBJECT_HPP
