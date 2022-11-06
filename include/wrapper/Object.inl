//
// Created by arthur on 03/11/2022.
//

#ifndef CONCERTOGRAPHICS_OBJECT_INL
#define CONCERTOGRAPHICS_OBJECT_INL

#include <utility>
#include <cassert>
#include "wrapper/Object.hpp"

namespace Concerto::Graphics::Wrapper
{
	template<typename vkType>
	Object<vkType>::Object(Device& device) : _device(&device)
	{

	}

	template<typename vkType>
	Object<vkType>::Object(Device& device, std::function<void()>&& destroyHelper) : _device(&device),
																					_destroyHelper(std::move(destroyHelper))

	{
	}
	template<typename vkType>
	Object<vkType>::~Object()
	{
		if (!IsNull() && _destroyHelper)
			_destroyHelper();
	}

	template<typename vkType>
	Object<vkType>::Object(Object&& other) noexcept
	{
		_handle = std::exchange(other._handle, VK_NULL_HANDLE);
		_device = std::exchange(other._device, nullptr);
	}

	template<typename vkType>
	Object<vkType>& Object<vkType>::operator=(Object&& other) noexcept
	{
		_handle = std::exchange(other._handle, VK_NULL_HANDLE);
		_device = std::exchange(other._device, nullptr);
		return *this;
	}

	template<typename vkType>
	vkType* Object<vkType>::Get() const
	{
		assert(!IsNull());
		return const_cast<vkType*>(&_handle);
	}

	template<typename vkType>
	bool Object<vkType>::IsNull() const
	{
		return _handle == VK_NULL_HANDLE;
	}
}

#endif //CONCERTOGRAPHICS_OBJECT_INL
