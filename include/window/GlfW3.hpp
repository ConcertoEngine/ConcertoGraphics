//
// Created by arthur on 23/05/22.
//

#ifndef CONCERTO_GLFW3_HPP
#define CONCERTO_GLFW3_HPP

#include "AWindow.hpp"
#if __linux__
#define GLFW_EXPOSE_NATIVE_X11
#elif _WIN32
#define GLFW_EXPOSE_NATIVE_WIN32
#elif __APPLE__
#define GLFW_EXPOSE_NATIVE_COCOA
#endif

#include <memory>
#include <functional>
#include <GLFW/glfw3.h>
#include <GLFW/glfw3native.h>

namespace Concerto
{
	class GlfW3 : public AWindow
	{
	public:
		void* getRawWindow() override;

		GlfW3(const std::string& title, unsigned int width, unsigned int height);

		GlfW3() = delete;

		GlfW3(GlfW3&&) = default;

		GlfW3(const GlfW3&) = delete;

		GlfW3& operator=(GlfW3&&) = default;

		GlfW3& operator=(const GlfW3&) = delete;

		~GlfW3() override;

		std::size_t getWidth() final;

		std::size_t getHeight() final;
		//Math::Vector2d getCursorPosition() override;

		void setTitle(const std::string& title) override;

		void setIcon(const std::string& path) override;

		void setCursorVisible(bool visible) override;

		void setCursorPosition(int x, int y) override;

		void setCursorIcon(const std::string& path) override;

		void setCursorDisabled(bool disabled) override;

		std::optional<Concerto::Key> popEvent() override;

	private:
		std::unique_ptr<GLFWwindow, std::function<void(GLFWwindow*)>> _window;
	};

} // Concerto

#endif //CONCERTO_GLFW3_HPP
