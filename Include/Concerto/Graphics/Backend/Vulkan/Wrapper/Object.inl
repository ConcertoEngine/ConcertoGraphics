//
// Created by arthur on 03/11/2022.
//

#ifndef CONCERTO_GRAPHICS_OBJECT_INL
#define CONCERTO_GRAPHICS_OBJECT_INL

#include <utility>

#include <Concerto/Core/Assert.hpp>
#include <Concerto/Core/TypeInfo.hpp>

#include "Concerto/Graphics/Backend/Vulkan/Wrapper/Object.hpp"

namespace cct::gfx::vk
{
	template<typename VkType>
	Object<VkType>::Object(Device& device) :
#ifdef CCT_ENABLE_OBJECT_DEBUG
	ObjectDebug(device, TypeName<std::remove_pointer_t<std::remove_cvref_t<VkType>>>(), reinterpret_cast<void**>(&this->_handle)),
#endif
		_handle(nullptr),
		_device(&device),
		_lastResult(VK_SUCCESS)
	{

	}

	template <typename VkType>
	Object<VkType>::~Object()
	{
#ifdef CCT_ENABLE_OBJECT_DEBUG
		_handle = 0xDDDDDDDD;
#endif // CCT_ENABLE_OBJECT_DEBUG
	}

	template<typename VkType>
	Object<VkType>::Object(Object&& other) noexcept
#ifdef CCT_ENABLE_OBJECT_DEBUG
		:
	ObjectDebug(std::move(other))
#endif
	{
		_handle = std::exchange(other._handle, nullptr);
		_device = std::exchange(other._device, nullptr);
		_lastResult = std::exchange(other._lastResult, VK_SUCCESS);
	}

	template<typename VkType>
	Object<VkType>& Object<VkType>::operator=(Object&& other) noexcept
	{
		std::swap(_handle, other._handle);
		std::swap(_device, other._device);
#ifdef CCT_ENABLE_OBJECT_DEBUG
		ObjectDebug::operator=(std::move(other));
#endif
		return *this;
	}

	template<typename VkType>
	VkType* Object<VkType>::Get() const
	{
		CCT_ASSERT(!IsNull(), "The vulkan object handle is null");
		return const_cast<VkType*>(&_handle);
	}

	template<typename VkType>
	bool Object<VkType>::IsNull() const
	{
		return _handle == nullptr;
	}

	template<typename VkType>
	Device* Object<VkType>::GetDevice() const
	{
		CCT_ASSERT(_device, "Invalid device");
		return _device;
	}

	template<typename VkType>
	VkResult Object<VkType>::GetLastResult() const
	{
		return _lastResult;
	}
}

#endif //CONCERTO_GRAPHICS_OBJECT_INL
