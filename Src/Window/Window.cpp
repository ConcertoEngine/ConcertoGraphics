//
// Created by arthur on 23/05/22.
//

#include <stdexcept>

#include <Concerto/Core/Logger.hpp>
#include <Concerto/Core/Assert.hpp>
#include <SDL2/SDL.h>
#include <SDL2/SDL_syswm.h>

#include "Concerto/Graphics/Window/Window.hpp"
#include "Concerto/Graphics/Window/Event.hpp"
namespace Concerto::Graphics
{
	void ErrorCallback(int ec, const char* description)
	{
		Logger::Error("GLFW Error: code {}, description '{}'", std::to_string(ec), std::string(description));
	}

	Window::Window(Int32 displayIndex, const std::string& title, Int32 width, Int32 height) :
		_title(title),
		_width(width),
		_height(height),
		_window(nullptr)
	{
		Uint32 flags = 0;
		flags |= SDL_WINDOW_RESIZABLE;
		_window = SDL_CreateWindow(title.c_str(), SDL_WINDOWPOS_CENTERED_DISPLAY(displayIndex), SDL_WINDOWPOS_CENTERED_DISPLAY(displayIndex), width, height, flags);
		if (_window == nullptr)
		{
			CONCERTO_ASSERT_FALSE("ConcertoGraphics: Window creation failed message: {}", SDL_GetError());
			throw std::runtime_error(std::format("ConcertoGraphics: Window creation failed message: {}", SDL_GetError()));
		}

		RegisterInputCallbacks();
	}
	Window::~Window()
	{
		SDL_DestroyWindow(_window);
		_window = nullptr;
	}

	void Window::SetTitle(const std::string& title)
	{
		CONCERTO_ASSERT(_window, "ConcertoGraphics: invalid window pointer");
		SDL_SetWindowTitle(_window, title.c_str());
	}

	void Window::SetCursorVisible(bool visible)
	{
		CONCERTO_ASSERT(_window, "ConcertoGraphics: invalid window pointer");
		Int32 result = SDL_ShowCursor(visible);
		if (result < 0)
			Logger::Warning("{}", SDL_GetError());
	}

	void Window::SetCursorIcon(const std::string& path)
	{
		CONCERTO_ASSERT(_window, "ConcertoGraphics: invalid window pointer");
	}

	void Window::SetCursorDisabled(bool disabled)
	{
		CONCERTO_ASSERT(_window, "ConcertoGraphics: invalid window pointer");
		if (disabled) {
			Int32 result = SDL_ShowCursor(SDL_DISABLE);
			if (result < 0)
				Logger::Warning("{}", SDL_GetError());
			result = SDL_SetRelativeMouseMode(SDL_TRUE);
			if (result < 0)
				Logger::Warning("{}", SDL_GetError());
		}
		else {
			Int32 result = SDL_ShowCursor(SDL_ENABLE);
			if (result < 0)
				Logger::Warning("{}", SDL_GetError());
			result = SDL_SetRelativeMouseMode(SDL_FALSE);
			if (result < 0)
				Logger::Warning("{}", SDL_GetError());
		}
	}

	UInt32 Window::GetHeight() const
	{
		CONCERTO_ASSERT(_window, "ConcertoGraphics: invalid window pointer");
		int w, h;
		SDL_GetWindowSize(_window, &w, &h);
		return static_cast<UInt32>(h);
	}

	UInt32 Window::GetWidth() const
	{
		CONCERTO_ASSERT(_window, "ConcertoGraphics: invalid window pointer");
		int w, h;
		SDL_GetWindowSize(_window, &w, &h);
		return static_cast<UInt32>(w);
	}

	NativeWindow Window::GetNativeWindow() const
	{
		CONCERTO_ASSERT(_window, "ConcertoGraphics: invalid window pointer");
		SDL_SysWMinfo wmInfo;
		SDL_VERSION(&wmInfo.version)
		NativeWindow nativeWindow;
		if (SDL_GetWindowWMInfo(_window, &wmInfo)) {
#if defined(CONCERTO_PLATFORM_WINDOWS)
			nativeWindow.window = wmInfo.info.win.window;
			nativeWindow.hinstance= wmInfo.info.win.hinstance;
#elif defined(CONCERTO_PLATFORM_MACOS))
			CONCERTO_ASSERT_FALSE("Not implemented");
#elif defined(CONCERTO_PLATFORM_LINUX)
			CONCERTO_ASSERT_FALSE("Not implemented");
#endif
		}
		else {
			CONCERTO_ASSERT_FALSE("ConcertoGraphics: Could not get native window handle message:", SDL_GetError());
			return {};
		}
		return nativeWindow;
	}

	std::optional<Key> Window::PopEvent()
	{
		return {};
	}

	bool Window::ShouldClose() const
	{
		CONCERTO_ASSERT(_window, "ConcertoGraphics: invalid window pointer");
		//return glfwWindowShouldClose(_window);
		return false;
	}

	void Window::RegisterResizeCallback(std::function<void(Window& window)> callback)
	{
		CONCERTO_ASSERT(_window, "ConcertoGraphics: invalid window pointer");
		_resizeCallback = std::move(callback);
		//glfwSetWindowUserPointer(_window, this);
		//glfwSetWindowSizeCallback(_window, [](GLFWwindow* window, int width, int height)
		//{
		//	auto* self = static_cast<Window*>(glfwGetWindowUserPointer(window));
		//	self->_width = width;
		//	self->_height = height;
		//	self->_resizeCallback(*self);
		//});
	}

	void Window::RegisterKeyCallback(std::function<void(Window&, Key, int, int, int)> callback)
	{
		CONCERTO_ASSERT(_window, "ConcertoGraphics: invalid window pointer");
		_keyCallback = callback;
		//glfwSetWindowUserPointer(_window, this);
		//glfwSetKeyCallback(_window, [](GLFWwindow* window, int key, int scancode, int action, int mods)
		//{
		//	auto* self = static_cast<Window*>(glfwGetWindowUserPointer(window));
		//	self->_keyCallback(*self, static_cast<Key>(key), scancode, action, mods);
		//});
	}

	void Window::RegisterMouseButtonCallback(std::function<void(Window& window, int button, int action, int mods)> callback)
	{
		CONCERTO_ASSERT(_window, "ConcertoGraphics: invalid window pointer");
		_mouseButtonCallback = std::move(callback);
		//glfwSetWindowUserPointer(_window, this);
		//glfwSetMouseButtonCallback(_window, [](GLFWwindow* window, int button, int action, int mods)
		//{
		//	auto* self = static_cast<Window*>(glfwGetWindowUserPointer(window));
		//	self->_mouseButtonCallback(*self, button, action, mods);
		//});
	}

	void Window::RegisterCursorPosCallback(std::function<void(Window& window, double xpos, double ypos)> callback)
	{
		CONCERTO_ASSERT(_window, "ConcertoGraphics: invalid window pointer");
		_cursorPosCallback = std::move(callback);
		//glfwSetWindowUserPointer(_window, this);
		//glfwSetCursorPosCallback(_window, [](GLFWwindow* window, double xpos, double ypos)
		//{
		//	auto* self = static_cast<Window*>(glfwGetWindowUserPointer(window));
		//	self->_cursorPosCallback(*self, xpos, ypos);
		//});
	}

	Input& Window::GetInputManager()
	{
		return _input;
	}

	void Window::RegisterInputCallbacks()
	{
		RegisterCursorPosCallback([&](Window& window, double x, double y)
		{
			//imGui->UpdateMousePosition(x, y);
			MouseEvent mouseEvent{};
			static double deltaX = 0, deltaY = 0;
			deltaX = x - deltaX;
			deltaY = y - deltaY;
			mouseEvent.type = MouseEvent::Type::Moved;
			mouseEvent.mouseMove.x = x;
			mouseEvent.mouseMove.y = y;
			mouseEvent.mouseMove.deltaX = deltaX;
			mouseEvent.mouseMove.deltaY = deltaY;
			_input.TriggerMouseEvent(mouseEvent);
			deltaX = x;
			deltaY = y;
		});

		RegisterMouseButtonCallback([&](Window& window, int button, int action, int mods)
		{
			//imGui->UpdateMouseButton(button, action == GLFW_PRESS);
			MouseEvent mouseEvent{};
			mouseEvent.type = MouseEvent::Type::Button;
			mouseEvent.button.button = static_cast<MouseButton::Button>(button);
			mouseEvent.button.triggerType = static_cast<TriggerType>(action);
			_input.TriggerMouseEvent(mouseEvent);
		});

		RegisterKeyCallback([&](Window& window, Key key, int scanCode, int action, int mods)
		{
			KeyEvent keyEvent{};
			keyEvent.key = key;
			keyEvent.triggerType = static_cast<TriggerType>(action);
			_input.TriggerKeyEvent(keyEvent);
		});

	}
}
