//
// Created by arthur on 23/05/22.
//


#include "window/GlfW3.hpp"
#include <stdexcept>

Concerto::GlfW3::GlfW3(const std::string& title, unsigned int width, unsigned int height) : AWindow(title, width,
		height)
{
	if (!glfwInit())
		throw std::runtime_error("GLFW3 initialization failed");
	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
	auto* window = glfwCreateWindow(width, height, title.c_str(), nullptr, nullptr);;
	if (!window)
		throw std::runtime_error("GLFW3 window creation failed");
	_window = std::unique_ptr<GLFWwindow, std::function<void(GLFWwindow*)>>(window, [](GLFWwindow* window)
	{
		glfwDestroyWindow(window);
	});
}

Concerto::GlfW3::~GlfW3()
{
	glfwTerminate();
}

//Concerto::Math::Vector2d Concerto::GlfW3::getCursorPosition()
//{
//	double x, y;
//	glfwGetCursorPos(_window.get(), &x, &y);
//	return Math::Vector2d(x, y);
//}

void Concerto::GlfW3::setTitle(const std::string& title)
{
	glfwSetWindowTitle(_window.get(), title.c_str());
}

void Concerto::GlfW3::setIcon(const std::string& path)
{

}

void Concerto::GlfW3::setCursorVisible(bool visible)
{
	if (visible)
		glfwSetInputMode(_window.get(), GLFW_CURSOR, GLFW_CURSOR_NORMAL);
	else glfwSetInputMode(_window.get(), GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
}

void Concerto::GlfW3::setCursorPosition(int x, int y)
{
	glfwSetCursorPos(_window.get(), x, y);
}

void Concerto::GlfW3::setCursorIcon(const std::string& path)
{

}

void Concerto::GlfW3::setCursorDisabled(bool disabled)
{
	if(disabled)
		glfwSetInputMode(_window.get(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	else glfwSetInputMode(_window.get(), GLFW_CURSOR, GLFW_CURSOR_NORMAL);
}

std::size_t Concerto::GlfW3::getHeight()
{
	int w, h;
	glfwGetWindowSize(_window.get(), &w, &h);
	return h;
}

std::size_t Concerto::GlfW3::getWidth()
{
	int w, h;
	glfwGetWindowSize(_window.get(), &w, &h);
	return w;
}

void* Concerto::GlfW3::getRawWindow()
{
	return _window.get();
}

