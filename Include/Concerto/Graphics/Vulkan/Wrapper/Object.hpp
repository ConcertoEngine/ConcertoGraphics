//
// Created by arthur on 03/11/2022.
//

#ifndef CONCERTOGRAPHICS_OBJECT_HPP
#define CONCERTOGRAPHICS_OBJECT_HPP

#include <functional>
#include <vulkan/vulkan.h>
#include <Concerto/Core/Types.hpp>

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
		using DestroyHelper = std::function<void(Device&, vkType)>;
		/**
		 * @brief Default constructor.
		 * @attention If you need to clean up the object by calling a Vulkan function, you mustn't use this constructor.
		*/
		explicit Object(Device& device);

		/**
		 * @brief Constructor.
		 * @param device The device.
		 * @param destroyHelper The function that will be called to clean up the object.
		 */
		explicit Object(Device& device, DestroyHelper&& destroyHelper);

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

		/**
		 * @brief Get the last result of the object
		 * @return The last result of the object
		 */
		[[nodiscard]] VkResult GetLastResult() const;

	protected:
		vkType _handle{ VK_NULL_HANDLE };
		Device* _device{ nullptr };
		VkResult _lastResult{ VK_SUCCESS };
	private:
		DestroyHelper _destroyHelper;
	};
}

#include "Object.inl"

#endif //CONCERTOGRAPHICS_OBJECT_HPP
