//
// Created by arthur on 23/05/22.
//


#include "window/GlfW3.hpp"
#include <stdexcept>
namespace Concerto::Graphics
{
	GlfW3::GlfW3(const std::string& title, int width, int height) : AWindow(title, width, height)
	{
		if (!glfwInit())
			throw std::runtime_error("GLFW3 initialization failed");
		glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
		auto* window = glfwCreateWindow(width, height, title.c_str(), nullptr, nullptr);;
		if (!window)
			throw std::runtime_error("GLFW3 Window creation failed");
		_window = std::unique_ptr<GLFWwindow, std::function<void(GLFWwindow*)>>(window, [](GLFWwindow* window)
		{
			glfwDestroyWindow(window);
		});
	}
	GlfW3::~GlfW3()
	{
		glfwTerminate();
	}

	void GlfW3::SetTitle(const std::string& title)
	{
		glfwSetWindowTitle(_window.get(), title.c_str());
	}

	void GlfW3::SetIcon(const std::string& path)
	{

	}

	void GlfW3::SetCursorVisible(bool visible)
	{
		if (visible)
			glfwSetInputMode(_window.get(), GLFW_CURSOR, GLFW_CURSOR_NORMAL);
		else glfwSetInputMode(_window.get(), GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
	}

	void GlfW3::SetCursorPosition(int x, int y)
	{
		glfwSetCursorPos(_window.get(), x, y);
	}

	void GlfW3::SetCursorIcon(const std::string& path)
	{

	}

	void GlfW3::SetCursorDisabled(bool disabled)
	{
		if (disabled)
			glfwSetInputMode(_window.get(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);
		else glfwSetInputMode(_window.get(), GLFW_CURSOR, GLFW_CURSOR_NORMAL);
	}

	std::uint32_t GlfW3::GetHeight()
	{
		int w, h;
		glfwGetWindowSize(_window.get(), &w, &h);
		return h;
	}

	std::uint32_t GlfW3::GetWidth()
	{
		int w, h;
		glfwGetWindowSize(_window.get(), &w, &h);
		return w;
	}

	void* GlfW3::GetRawWindow()
	{
		return _window.get();
	}

	std::optional<Key> GlfW3::PopEvent()
	{
		glfwPollEvents();
		return {};
	}

	bool GlfW3::ShouldClose()
	{
		return glfwWindowShouldClose(_window.get());
	}

	void GlfW3::RegisterResizeCallback(std::function<void(AWindow& window)> callback)
	{
		_resizeCallback = callback;
		glfwSetWindowUserPointer(_window.get(), this);
		glfwSetWindowSizeCallback(_window.get(), [](GLFWwindow* window, int width, int height)
		{
			auto* self = static_cast<GlfW3*>(glfwGetWindowUserPointer(window));
			self->_width = width;
			self->_height = height;
			self->_resizeCallback(*self);
		});
	}

	void GlfW3::RegisterKeyCallback(std::function<void(AWindow&, Key, int, int, int)> callback)
	{
		_keyCallback = callback;
		glfwSetWindowUserPointer(_window.get(), this);
		glfwSetKeyCallback(_window.get(), [](GLFWwindow* window, int key, int scancode, int action, int mods)
		{
			auto* self = static_cast<GlfW3*>(glfwGetWindowUserPointer(window));
			self->_keyCallback(*self, static_cast<Key>(key), scancode, action, mods);
		});

	}

	void GlfW3::RegisterMouseButtonCallback(std::function<void(AWindow& window, int button, int action, int mods)> callback)
	{
		_mouseButtonCallback = callback;
		glfwSetWindowUserPointer(_window.get(), this);
		glfwSetMouseButtonCallback(_window.get(), [](GLFWwindow* window, int button, int action, int mods)
		{
			auto* self = static_cast<GlfW3*>(glfwGetWindowUserPointer(window));
			self->_mouseButtonCallback(*self, button, action, mods);
		});
	}

	void GlfW3::RegisterCursorPosCallback(std::function<void(AWindow& window, double xpos, double ypos)> callback)
	{
		_cursorPosCallback = callback;
		glfwSetWindowUserPointer(_window.get(), this);
		glfwSetCursorPosCallback(_window.get(), [](GLFWwindow* window, double xpos, double ypos)
		{
			auto* self = static_cast<GlfW3*>(glfwGetWindowUserPointer(window));
			self->_cursorPosCallback(*self, xpos, ypos);
		});
	}
}
