//
// Created by arthur on 23/05/22.
//

#ifndef CONCERTO_GRAPHICS_WINDOW_HPP
#define CONCERTO_GRAPHICS_WINDOW_HPP

#include <string>
#include <functional>

#include "Concerto/Graphics/Core/Defines.hpp"
#include "Concerto/Graphics/Core/Window/Key.hpp"
#include "Concerto/Graphics/Core/Input/Input.hpp"
#include "Concerto/Graphics/Core/Window/NativeWindow.hpp"

struct SDL_Window;

namespace cct::gfx
{

	class CONCERTO_GRAPHICS_CORE_API Window
	{
	public:
		Window(Int32 displayIndex, const std::string& title, Int32 width, Int32 height);

		Window() = delete;

		Window(Window&&) noexcept = default;

		Window(const Window&) = delete;

		Window& operator=(Window&&) noexcept = default;

		Window& operator=(const Window&) = delete;

		~Window();

		/**
		* @brief Get the raw Window
		* @return The raw Window as a void pointer
		*/
		NativeWindow GetNativeWindow() const;
		/**
		 * @return The cursor position in the Window
		 */
		//Math::Vector2d getCursorPosition();
		/**
		 * @return The width of the Window
		 */
		UInt32 GetWidth() const;

		/**
		 * @return The height of the Window
		 */
		UInt32 GetHeight() const;

		/**
		 * @bried Set the Window title
		 * @param title The new title
		 */
		void SetTitle(const std::string& title);

		/**
		 * @brief Set the cursor visibility
		 * @param visible True if the cursor should be visible, false otherwise
		 */
		void SetCursorVisible(bool visible);

		/**
		 * @brief Set the cursor icon
		 * @param path The path to the cursor icon
		 */
		void SetCursorIcon(const std::string& path);

		/**
		 * @brief The cursor always go back to the center of the Window, and becomes invisible
		 * @param disabled True if the cursor should be disabled, false otherwise
		 */
		void SetCursorDisabled(bool disabled);

		/**
		 * @brief Check if the Window should close
		 * @return True if the Window should close, false otherwise
		 */
		bool ShouldClose() const;

		/**
		 * @brief Register a callback function that will be called when the Window is resized
		 * @param callback The callback function
		 */
		void RegisterResizeCallback(std::function<void(Window& window)> callback);

		/**
		 * @brief Register a callback function that will be called when a key is pressed
		 * @param callback The callback function
		 */
		void RegisterKeyCallback(std::function<void(Window& window, Key button, int scancode, int action, int mods)> callback);

		/**
		 * @brief Register a callback function that will be called when a mouse button is pressed
		 * @param callback The callback function
		 */
		void RegisterMouseButtonCallback(std::function<void(Window& window, int button, int action, int mods)> callback);

		/**
		 * @brief Register a callback function that will be called when the mouse cursor has moved
		 * @param callback The callback function
		 */
		void RegisterCursorPosCallback(std::function<void(Window& window, double xpos, double ypos)> callback);

		Input& GetInputManager();

		UInt32 GetId() const;

		void SetShouldQuit(bool value);
		void TriggerResize();
	private:
		void RegisterInputCallbacks();
		std::string m_title;
		std::size_t m_width;
		std::size_t m_height;
		SDL_Window* m_window;
		Input m_input;
		std::function<void(Window& window)> m_resizeCallback;
		std::function<void(Window& window, Key key, int scancode, int action, int mods)> m_keyCallback;
		std::function<void(Window& window, int button, int action, int mods)> m_mouseButtonCallback;
		std::function<void(Window& window, double xpos, double ypos)> m_cursorPosCallback;
		UInt32 m_windowID;
		bool m_shouldQuit;
	};
}

#endif //CONCERTO_GRAPHICS_WINDOW_HPP