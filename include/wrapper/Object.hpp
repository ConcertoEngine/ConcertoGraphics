//
// Created by arthur on 03/11/2022.
//

#ifndef CONCERTOGRAPHICS_OBJECT_HPP
#define CONCERTOGRAPHICS_OBJECT_HPP

#include "vulkan/vulkan.h"

namespace Concerto::Graphics::Wrapper
{
	class Device;

	template<typename vkType>
	class Object
	{
	public:
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

	protected:
		vkType _handle{ VK_NULL_HANDLE };
		Device* _device{ nullptr };
	};
}

#include "Object.inl"

#endif //CONCERTOGRAPHICS_OBJECT_HPP
