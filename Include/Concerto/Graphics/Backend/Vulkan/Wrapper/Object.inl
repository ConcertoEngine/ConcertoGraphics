//
// Created by arthur on 03/11/2022.
//

#ifndef CONCERTO_GRAPHICS_OBJECT_INL
#define CONCERTO_GRAPHICS_OBJECT_INL

#include <utility>

#include <Concerto/Core/Assert.hpp>
#include <Concerto/Core/TypeInfo.hpp>

#include "Concerto/Graphics/Backend/Vulkan/Wrapper/Object.hpp"

namespace Concerto::Graphics::Vk
{
	template<typename VkType>
	Object<VkType>::Object(Device& device) :
#ifdef CONCERTO_DEBUG
	ObjectDebug(device, TypeName<std::remove_pointer_t<std::remove_cvref_t<VkType>>>(), reinterpret_cast<void**>(&this->_handle)),
#endif
		_handle(nullptr),
		_device(&device),
		_lastResult(VK_SUCCESS)
	{

	}

	template<typename VkType>
	Object<VkType>::Object(Object&& other) noexcept
#ifdef CONCERTO_DEBUG
		:
	ObjectDebug(*other.GetDevice(), TypeName<std::remove_pointer_t<std::remove_cvref_t<VkType>>>(), reinterpret_cast<void**>(&this->_handle))
#endif
	{
		_handle = std::exchange(other._handle, nullptr);
		_device = std::exchange(other._device, nullptr);
		_lastResult = std::exchange(other._lastResult, VK_SUCCESS);
	}

	template<typename VkType>
	Object<VkType>& Object<VkType>::operator=(Object&& other) noexcept
	{
		_handle = std::exchange(other._handle, nullptr);
		_device = std::exchange(other._device, nullptr);
		return *this;
	}

	template<typename VkType>
	VkType* Object<VkType>::Get() const
	{
		CONCERTO_ASSERT(!IsNull(), "The vulkan object handle is null");
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
		CONCERTO_ASSERT(_device, "Invalid device");
		return _device;
	}

	template<typename VkType>
	VkResult Object<VkType>::GetLastResult() const
	{
		return _lastResult;
	}
}

#endif //CONCERTO_GRAPHICS_OBJECT_INL
