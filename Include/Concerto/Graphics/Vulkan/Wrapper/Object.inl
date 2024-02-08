//
// Created by arthur on 03/11/2022.
//

#ifndef CONCERTO_GRAPHICS_OBJECT_INL
#define CONCERTO_GRAPHICS_OBJECT_INL

#include <utility>

#include <Concerto/Core/Assert.hpp>

#include "Concerto/Graphics/Vulkan/Wrapper/Object.hpp"

namespace Concerto::Graphics
{
	template<typename VkType>
	Object<VkType>::Object(Device& device) : _device(&device)
	{

	}

	template<typename VkType>
	Object<VkType>::Object(Device& device, DestroyHelper&& destroyHelper) :
		_device(&device),
		_destroyHelper(std::move(destroyHelper))
	{
	}

	template<typename VkType>
	Object<VkType>::~Object()
	{
		if (!IsNull() && _device != nullptr && _destroyHelper)
			_destroyHelper(*_device, _handle);
	}

	template<typename VkType>
	Object<VkType>::Object(Object&& other) noexcept
	{
		_handle = std::exchange(other._handle, VK_NULL_HANDLE);
		_device = std::exchange(other._device, nullptr);
		if (other._destroyHelper)
			_destroyHelper = std::exchange(other._destroyHelper, nullptr);
	}

	template<typename VkType>
	Object<VkType>& Object<VkType>::operator=(Object&& other) noexcept
	{
		_handle = std::exchange(other._handle, VK_NULL_HANDLE);
		_device = std::exchange(other._device, nullptr);
		if (other._destroyHelper)
			_destroyHelper = std::exchange(other._destroyHelper, nullptr);
		return *this;
	}

	template<typename VkType>
	VkType* Object<VkType>::Get() const
	{
		CONCERTO_ASSERT(!IsNull());
		return const_cast<VkType*>(&_handle);
	}

	template<typename VkType>
	bool Object<VkType>::IsNull() const
	{
		return _handle == VK_NULL_HANDLE;
	}

	template<typename VkType>
	VkResult Object<VkType>::GetLastResult() const
	{
		return _lastResult;
	}
}

#endif //CONCERTO_GRAPHICS_OBJECT_INL
