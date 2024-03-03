//
// Created by arthur on 03/11/2022.
//

#ifndef CONCERTO_GRAPHICS_OBJECT_HPP
#define CONCERTO_GRAPHICS_OBJECT_HPP

#include <functional>
#include <vulkan/vulkan.h>

namespace Concerto::Graphics
{
	class Device;

	/**
	 * @brief `Object` is a template class that represents a Vulkan object.
	 * This class is intended to be used as a base class for representing specific types of Vulkan objects.
	 * @tparam vkType The type of the Vulkan object.
	 */
	template<typename vkType>
	class Object
	{
	public:
		/**
		 * @brief Default constructor.
		*/
		explicit Object(Device& device);

		Object(const Object&) = delete;

		Object(Object&&) noexcept;

		Object& operator=(const Object&) = delete;

		Object& operator=(Object&&) noexcept;

		~Object() = default;

		/**
		 * @brief Get the handle of the object
		 * @return The handle of the object
		 */
		vkType* Get() const;

		/**
		 * @brief Check if the object is null
		 * @return true if the object is null
		 */
		[[nodiscard]] bool IsNull() const;

		/**
		 * @brief Get the last result of the object
		 * @return The last result of the object
		 */
		[[nodiscard]] VkResult GetLastResult() const;

	protected:
		vkType _handle;
		Device* _device;
		mutable VkResult _lastResult;
	};
}

#include "Object.inl"

#endif //CONCERTO_GRAPHICS_OBJECT_HPP
