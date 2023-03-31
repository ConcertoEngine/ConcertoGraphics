//
// Created by arthur on 17/08/2022.
//

#include <cassert>
#include <iostream>
#include "Input.hpp"

namespace Concerto
{
	Input* Input::_instance = nullptr;

	Input::Input()
	{
		assert(!_instance);
		_instance = this;
	}

	Input& Input::Instance()
	{
		return *_instance;
	}

	Input::~Input()
	{
		assert(_instance);
		_instance = nullptr;
	}

	void Input::Register(const std::string& name, Key key, TriggerType triggerType,
			std::function<void(float deltaTime)>&& callback)
	{
		auto it = _keyCallbacks.find(name);
		if (it == _keyCallbacks.end())
		{
			auto pair = std::make_pair(SparseArray<bool>(), TriggerTypeCallbacks());
			pair.first.Emplace((std::size_t)key, true);
			pair.second[triggerType].push_back(std::move(callback));
			_keyCallbacks.emplace(name, std::move(pair));
			return;
		}
		it->second.first.Emplace((std::size_t)key, true);
		it->second.second[triggerType].push_back(std::move(callback));
	}

	void
	Input::Register(const std::string& name, MouseEvent::Type key, std::function<void(const MouseEvent&, float deltaTime)>&& callback)
	{
		auto it = _mouseCallback.find(name);
		if (it == _mouseCallback.end())
		{
			std::vector<MouseEventCallbacks> vec = { std::move(callback) };
			auto pair = std::make_pair(key, std::move(vec));
			_mouseCallback.emplace(name, std::move(pair));
			return;
		}
		it->second.second.push_back(std::move(callback));
	}

	void Input::Trigger(const std::vector<Event>& events, float deltaTime)
	{
		for (const auto& event: events)
		{
			if (event.type == Event::Type::Key)
				TriggerKeyEvent(std::get<KeyEvent>(event.data), deltaTime);
			else if (event.type == Event::Type::Mouse)
				TriggerMouseEvent(std::get<MouseEvent>(event.data), deltaTime);
		}
	}

	void Input::TriggerKeyEvent(const KeyEvent& keyEvent, float deltaTime)
	{
		for (auto& [key, bindingCallback]: _keyCallbacks)
		{
			auto keyIndex = (std::size_t)keyEvent.key;
			if (!bindingCallback.first.Has(keyIndex) || !bindingCallback.first[keyIndex])
				continue;
			auto it = bindingCallback.second.find(keyEvent.triggerType);
			if (it == bindingCallback.second.end())
				continue;
			for (auto& callback: it->second)
				callback(deltaTime);
		}
	}

	void Input::TriggerMouseEvent(const MouseEvent& mouseEvent, float deltaTime)
	{
		for (const auto& [key, callbacks]: _mouseCallback)
		{
			if (callbacks.first != mouseEvent.type)
				continue;
			for (auto& callback: callbacks.second)
				callback(mouseEvent, deltaTime);
		}
	}
}