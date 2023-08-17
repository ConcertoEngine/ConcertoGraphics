//
// Created by arthur on 23/05/22.
//

#ifndef CONCERTO_GLFW3_HPP
#define CONCERTO_GLFW3_HPP

#include "Concerto/Graphics/Window/AWindow.hpp"
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
#include <Concerto/Core/Types.hpp>

namespace Concerto::Graphics
{
	class CONCERTO_PUBLIC_API GlfW3 : public AWindow<GLFWwindow>
	{
	public:
		GlfW3(const std::string& title, int width, int height);

		void* GetRawWindow() override;

		GlfW3() = delete;

		GlfW3(GlfW3&&) = default;

		GlfW3(const GlfW3&) = delete;

		GlfW3& operator=(GlfW3&&) = default;

		GlfW3& operator=(const GlfW3&) = delete;

		~GlfW3() override;

		UInt32 GetWidth() final;

		UInt32 GetHeight() final;

		void SetTitle(const std::string& title) override;

		void SetIcon(const std::string& path) override;

		void SetCursorVisible(bool visible) override;

		void SetCursorPosition(int x, int y) override;

		void SetCursorIcon(const std::string& path) override;

		void SetCursorDisabled(bool disabled) override;

		std::optional<Key> PopEvent() override;

		bool ShouldClose() override;

		void RegisterResizeCallback(std::function<void(AWindow& window)> callback) override;

		void RegisterKeyCallback(std::function<void(AWindow& window, Key key, int scancode, int action, int mods)> callback) override;

		void RegisterMouseButtonCallback(std::function<void(AWindow& window, int button, int action, int mods)> callback) override;

		void RegisterCursorPosCallback(std::function<void(AWindow& window, double xpos, double ypos)> callback) override;
	private:
		std::unique_ptr<GLFWwindow, std::function<void(GLFWwindow*)>> _window;
	};
	using GLFW3WindowPtr = std::unique_ptr<GlfW3>;

} // Concerto

#endif //CONCERTO_GLFW3_HPP
