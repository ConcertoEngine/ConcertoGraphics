//
// Created by arthur on 23/05/22.
//

#include <stdexcept>

#include <Concerto/Core/Logger.hpp>
#include <Concerto/Core/Assert.hpp>
#include <utility>

#include "Concerto/Graphics/Window/Window.hpp"
#include "Concerto/Graphics/Backend/Vulkan/Wrapper/Instance.hpp"
#include "Concerto/Graphics/Window/Event.hpp"

#if __linux__
#define GLFW_EXPOSE_NATIVE_X11
#elif _WIN32
#define GLFW_EXPOSE_NATIVE_WIN32
#elif __APPLE__
#define GLFW_EXPOSE_NATIVE_COCOA
#endif
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <GLFW/glfw3native.h>

namespace Concerto::Graphics
{
	void ErrorCallback(int ec, const char* description)
	{
		Logger::Error("GLFW Error: code {}, description '{}'", std::to_string(ec), std::string(description));
	}

	Window::Window(const std::string& title, int width, int height) :
		_title(title),
		_width(width),
		_height(height),
		_window(nullptr)
	{
		glfwSetErrorCallback(ErrorCallback);
		if (glfwInit() == GLFW_FALSE)
		{
			CONCERTO_ASSERT_FALSE("ConcertoGraphics: Glfw initialization failed");
			throw std::runtime_error("GLFW3 initialization failed");
		}
		glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
		_window = glfwCreateWindow(width, height, title.c_str(), nullptr, nullptr);;
		if (!_window)
		{
			CONCERTO_ASSERT_FALSE("ConcertoGraphics: Window creation failed");
			throw std::runtime_error("GLFW3 Window creation failed");
		}
		RegisterInputCallbacks();
	}
	Window::~Window()
	{
		glfwDestroyWindow(_window);
		_window = nullptr;
		glfwTerminate();
	}

	bool Window::IsVulkanSupported()
	{
		return glfwVulkanSupported() == GLFW_TRUE;
	}

	void Window::SetTitle(const std::string& title)
	{
		CONCERTO_ASSERT(_window, "ConcertoGraphics: invalid window pointer");
		glfwSetWindowTitle(_window, title.c_str());
	}

	void Window::SetIcon(const std::string& path)
	{
		CONCERTO_ASSERT(_window, "ConcertoGraphics: invalid window pointer");
	}

	void Window::SetCursorVisible(bool visible)
	{
		CONCERTO_ASSERT(_window, "ConcertoGraphics: invalid window pointer");
		if (visible)
			glfwSetInputMode(_window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
		else glfwSetInputMode(_window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
	}

	void Window::SetCursorPosition(int x, int y)
	{
		CONCERTO_ASSERT(_window, "ConcertoGraphics: invalid window pointer");
		glfwSetCursorPos(_window, x, y);
	}

	void Window::SetCursorIcon(const std::string& path)
	{
		CONCERTO_ASSERT(_window, "ConcertoGraphics: invalid window pointer");
	}

	void Window::SetCursorDisabled(bool disabled)
	{
		CONCERTO_ASSERT(_window, "ConcertoGraphics: invalid window pointer");
		if (disabled)
			glfwSetInputMode(_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
		else glfwSetInputMode(_window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
	}

	UInt32 Window::GetHeight()
	{
		CONCERTO_ASSERT(_window, "ConcertoGraphics: invalid window pointer");
		int w, h;
		glfwGetWindowSize(_window, &w, &h);
		return h;
	}

	UInt32 Window::GetWidth()
	{
		CONCERTO_ASSERT(_window, "ConcertoGraphics: invalid window pointer");
		int w, h;
		glfwGetWindowSize(_window, &w, &h);
		return w;
	}

	bool Window::CreateVulkanSurface(VkInstance instance, VkSurfaceKHR& surface) const
	{
		const auto res = glfwCreateWindowSurface(instance, _window, nullptr, &surface);
		if (res != VK_SUCCESS)
		{
			Logger::Warning("Unable to create vulkan surface error code: {}",  std::to_string(res));
			return false;
		}
		return true;
	}

	void* Window::GetRawWindow()
	{
		CONCERTO_ASSERT(_window, "ConcertoGraphics: invalid window pointer");
		return _window;
	}

	std::optional<Key> Window::PopEvent()
	{
		glfwPollEvents();
		return {};
	}

	bool Window::ShouldClose()
	{
		CONCERTO_ASSERT(_window, "ConcertoGraphics: invalid window pointer");
		return glfwWindowShouldClose(_window);
	}

	void Window::RegisterResizeCallback(std::function<void(Window& window)> callback)
	{
		CONCERTO_ASSERT(_window, "ConcertoGraphics: invalid window pointer");
		_resizeCallback = std::move(callback);
		glfwSetWindowUserPointer(_window, this);
		glfwSetWindowSizeCallback(_window, [](GLFWwindow* window, int width, int height)
		{
			auto* self = static_cast<Window*>(glfwGetWindowUserPointer(window));
			self->_width = width;
			self->_height = height;
			self->_resizeCallback(*self);
		});
	}

	void Window::RegisterKeyCallback(std::function<void(Window&, Key, int, int, int)> callback)
	{
		CONCERTO_ASSERT(_window, "ConcertoGraphics: invalid window pointer");
		_keyCallback = callback;
		glfwSetWindowUserPointer(_window, this);
		glfwSetKeyCallback(_window, [](GLFWwindow* window, int key, int scancode, int action, int mods)
		{
			auto* self = static_cast<Window*>(glfwGetWindowUserPointer(window));
			self->_keyCallback(*self, static_cast<Key>(key), scancode, action, mods);
		});

	}

	void Window::RegisterMouseButtonCallback(std::function<void(Window& window, int button, int action, int mods)> callback)
	{
		CONCERTO_ASSERT(_window, "ConcertoGraphics: invalid window pointer");
		_mouseButtonCallback = std::move(callback);
		glfwSetWindowUserPointer(_window, this);
		glfwSetMouseButtonCallback(_window, [](GLFWwindow* window, int button, int action, int mods)
		{
			auto* self = static_cast<Window*>(glfwGetWindowUserPointer(window));
			self->_mouseButtonCallback(*self, button, action, mods);
		});
	}

	void Window::RegisterCursorPosCallback(std::function<void(Window& window, double xpos, double ypos)> callback)
	{
		CONCERTO_ASSERT(_window, "ConcertoGraphics: invalid window pointer");
		_cursorPosCallback = std::move(callback);
		glfwSetWindowUserPointer(_window, this);
		glfwSetCursorPosCallback(_window, [](GLFWwindow* window, double xpos, double ypos)
		{
			auto* self = static_cast<Window*>(glfwGetWindowUserPointer(window));
			self->_cursorPosCallback(*self, xpos, ypos);
		});
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
