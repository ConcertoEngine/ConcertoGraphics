//
// Created by arthur on 17/08/2022.
//

#ifndef CONCERTO_INPUT_HPP
#define CONCERTO_INPUT_HPP

#include <functional>
#include <unordered_map>
#include <string>
#include <vector>
#include <Concerto/Core/SparseArray.hpp>
#include <Concerto/Core/Types.hpp>
#include "Event.hpp"
#include "Key.hpp"

namespace Concerto
{
	class CONCERTO_PUBLIC_API Input
	{
	public:
		Input();

		Input(const Input&) = delete;

		Input(Input&&) = delete;

		Input& operator=(const Input&) = delete;

		Input& operator=(Input&&) = delete;

		~Input();

		static Input& Instance();

		void Register(const std::string& name, Key key, TriggerType triggerType, std::function<void(float deltaTime)>&& callback);

		void Register(const std::string& name, MouseEvent::Type key, std::function<void(const MouseEvent&, float deltaTime)>&& callback);

		void Trigger(const std::vector<Event>& events, float deltaTime);

		void TriggerKeyEvent(const KeyEvent& keyEvent, float deltaTime);

		void TriggerMouseEvent(const MouseEvent& mouseEvent, float deltaTime);


		static Input* _instance;
		using KeyCallbacks = std::vector<std::function<void(float deltaTime)>>;
		using TriggerTypeCallbacks = std::unordered_map<TriggerType, KeyCallbacks>;
		using BindingCallback = std::pair<SparseArray<bool>, TriggerTypeCallbacks>;
		std::unordered_map<std::string, BindingCallback> _keyCallbacks;

		using MouseEventCallbacks = std::function<void(const MouseEvent&, float)>;
		std::unordered_map<std::string, std::pair<MouseEvent::Type, std::vector<MouseEventCallbacks>>> _mouseCallback;
	};
}
#endif //CONCERTO_INPUT_HPP
