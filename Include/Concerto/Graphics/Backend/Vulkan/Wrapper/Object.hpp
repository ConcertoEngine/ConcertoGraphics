//
// Created by arthur on 03/11/2022.
//

#ifndef CONCERTO_GRAPHICS_OBJECT_HPP
#define CONCERTO_GRAPHICS_OBJECT_HPP

#include "Concerto/Graphics/Backend/Vulkan/Defines.hpp"
#include "Concerto/Graphics/Backend/Vulkan/Wrapper/ObjectDebug.hpp"

namespace cct::gfx::vk
{
	class Device;

	template<typename VkType>
	class Object
#ifdef CCT_ENABLE_OBJECT_DEBUG
		:
		public ObjectDebug
#endif
	{
	public:
		explicit Object(Device& device);

		Object(const Object&) = delete;

		virtual ~Object()
#ifdef CCT_ENABLE_OBJECT_DEBUG
		override
#endif // CCT_ENABLE_OBJECT_DEBUG
		;

		Object(Object&&) noexcept;

		Object& operator=(const Object&) = delete;

		Object& operator=(Object&&) noexcept;

		[[nodiscard]] VkType* Get() const;
		[[nodiscard]] bool IsNull() const;
		[[nodiscard]] Device* GetDevice() const;
		[[nodiscard]] VkResult GetLastResult() const;

	protected:
		VkType m_handle;
		Device* m_device;
		mutable VkResult m_lastResult;
	};
}

#include "Concerto/Graphics/Backend/Vulkan/Wrapper/Object.inl"

#endif //CONCERTO_GRAPHICS_OBJECT_HPP
