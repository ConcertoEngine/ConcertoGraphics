//
// Created by arthur on 03/11/2022.
//

#ifndef CONCERTOGRAPHICS_OBJECT_HPP
#define CONCERTOGRAPHICS_OBJECT_HPP

#include <functional>
#include "vulkan/vulkan.h"

namespace Concerto::Graphics::Wrapper
{
	class Device;

	template<typename vkType>
	class Object
	{
	public:
		explicit Object(Device& device);
		explicit Object(Device& device, std::function<void()>&& destroyHelper);
		Object(const Object&) = delete;

		Object(Object&&) noexcept;

		Object& operator=(const Object&) = delete;

		Object& operator=(Object&&) noexcept;

		~Object();

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
	private:
		std::function<void()> _destroyHelper;
	};
}

#include "Object.inl"

#endif //CONCERTOGRAPHICS_OBJECT_HPP
