//
// Created by arthur on 23/05/22.
//

#include <stdexcept>

#include <Concerto/Core/Logger.hpp>
#include "Concerto/Graphics/Window/Window.hpp"
#include "Concerto/Graphics/Vulkan/Wrapper/Instance.hpp"

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
		Logger::Error("GLFW Error: <" + std::to_string(ec) + "> " + std::string(description));
	}

	Window::Window(const std::string& title, int width, int height) :
		_title(title),
		_width(width),
		_height(height),
		_window(nullptr)
	{
		glfwSetErrorCallback(ErrorCallback);
		if (glfwInit() == GLFW_FALSE)
			throw std::runtime_error("GLFW3 initialization failed");
		glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
		_window = glfwCreateWindow(width, height, title.c_str(), nullptr, nullptr);;
		if (!_window)
			throw std::runtime_error("GLFW3 Window creation failed");

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
		CONCERTO_ASSERT(_window);
		glfwSetWindowTitle(_window, title.c_str());
	}

	void Window::SetIcon(const std::string& path)
	{
		CONCERTO_ASSERT(_window);
	}

	void Window::SetCursorVisible(bool visible)
	{
		CONCERTO_ASSERT(_window);
		if (visible)
			glfwSetInputMode(_window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
		else glfwSetInputMode(_window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
	}

	void Window::SetCursorPosition(int x, int y)
	{
		CONCERTO_ASSERT(_window);
		glfwSetCursorPos(_window, x, y);
	}

	void Window::SetCursorIcon(const std::string& path)
	{
		CONCERTO_ASSERT(_window);
	}

	void Window::SetCursorDisabled(bool disabled)
	{
		CONCERTO_ASSERT(_window);
		if (disabled)
			glfwSetInputMode(_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
		else glfwSetInputMode(_window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
	}

	UInt32 Window::GetHeight()
	{
		CONCERTO_ASSERT(_window);
		int w, h;
		glfwGetWindowSize(_window, &w, &h);
		return h;
	}

	UInt32 Window::GetWidth()
	{
		CONCERTO_ASSERT(_window);
		int w, h;
		glfwGetWindowSize(_window, &w, &h);
		return w;
	}

	bool Window::CreateVulkanSurface(Instance& instance, VkSurfaceKHR* surface)
	{
		auto res = glfwCreateWindowSurface(*instance.Get(), _window, nullptr, surface);
		if (res != VK_SUCCESS)
		{
			Logger::Warning("Unable to create vulkan surface error code: " + std::to_string(int(res)));
			return false;
		}
		return true;
	}

	void* Window::GetRawWindow()
	{
		CONCERTO_ASSERT(_window);
		return _window;
	}

	std::optional<Key> Window::PopEvent()
	{
		glfwPollEvents();
		return {};
	}

	bool Window::ShouldClose()
	{
		CONCERTO_ASSERT(_window);
		return glfwWindowShouldClose(_window);
	}

	void Window::RegisterResizeCallback(std::function<void(Window& window)> callback)
	{
		CONCERTO_ASSERT(_window);
		_resizeCallback = callback;
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
		CONCERTO_ASSERT(_window);
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
		CONCERTO_ASSERT(_window);
		_mouseButtonCallback = callback;
		glfwSetWindowUserPointer(_window, this);
		glfwSetMouseButtonCallback(_window, [](GLFWwindow* window, int button, int action, int mods)
		{
			auto* self = static_cast<Window*>(glfwGetWindowUserPointer(window));
			self->_mouseButtonCallback(*self, button, action, mods);
		});
	}

	void Window::RegisterCursorPosCallback(std::function<void(Window& window, double xpos, double ypos)> callback)
	{
		CONCERTO_ASSERT(_window);
		_cursorPosCallback = callback;
		glfwSetWindowUserPointer(_window, this);
		glfwSetCursorPosCallback(_window, [](GLFWwindow* window, double xpos, double ypos)
		{
			auto* self = static_cast<Window*>(glfwGetWindowUserPointer(window));
			self->_cursorPosCallback(*self, xpos, ypos);
		});
	}
}
