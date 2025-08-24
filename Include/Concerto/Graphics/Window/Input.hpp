//
// Created by arthur on 17/08/2022.
//

#ifndef CONCERTO_INPUT_HPP
#define CONCERTO_INPUT_HPP

#include <functional>
#include <unordered_map>
#include <string>
#include <vector>
#include <Concerto/Core/FunctionRef.hpp>

#include <Concerto/Core/SparseVector.hpp>

#include "Concerto/Graphics/Defines.hpp"
#include "Concerto/Graphics/Window/Event.hpp"
#include "Concerto/Graphics/Window/Key.hpp"

namespace cct
{
	class CONCERTO_GRAPHICS_API Input
	{
	public:
		using KeyCallbacks = std::vector<FunctionRef<void()>>;
		using TriggerTypeCallbacks = std::unordered_map<TriggerType, KeyCallbacks>;
		using BindingCallback = std::pair<SparseVector<bool>, TriggerTypeCallbacks>;
		using MouseEventCallback = FunctionRef<void(const MouseEvent&)>;

		Input() = default;

		~Input() = default;

		Input(const Input&) = delete;

		Input(Input&&) = default;

		Input& operator=(const Input&) = delete;

		Input& operator=(Input&&) = default;


		void Register(const std::string& name, Key key, TriggerType triggerType, FunctionRef<void()>&& callback);

		void Register(const std::string& name, MouseEvent::Type key, MouseEventCallback&& callback);

		void Trigger(const std::vector<Event>& events);

		void TriggerKeyEvent(const KeyEvent& keyEvent);

		void TriggerMouseEvent(const MouseEvent& mouseEvent);

	private:
		std::unordered_map<std::string, BindingCallback> m_keyCallbacks;
		std::unordered_map<std::string, std::pair<MouseEvent::Type, std::vector<MouseEventCallback>>> m_mouseCallback;
	};
}
#endif //CONCERTO_INPUT_HPP
